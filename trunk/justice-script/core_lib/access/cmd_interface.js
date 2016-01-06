require('../../../justice-cmd/command/nvme-cmd.js')
var fmt = require('printf')
var init = require('../../init/init.js')
var sprintf = require("sprintf-js").sprintf
var pat = require('../../core_lib/c_pattern.jclib')
var logs = require('../../core_lib/c_logs.jclib')
var w_buff = get_write_buffer()
var r_buff = get_read_buffer()
var out = require('../../output/output.js')
var MAX_BUFFER_SIZE = 64 * 1024 * 1024
var Q_DEPTH = 64
var Q_NUM = 8
var SECTOR_SIZE = init.sector_size
var TOTAL_BLK=init.nlbaf
//var q_depth = 20
var q_num = 1
var wr_comm_num = 0 
var wr_sector_num = 0
var rd_comm_num = 0
var rd_sector_num = 0 
var rd_res = 0
var wr_res = 0
var pre_cmpcount = 0
var pre_cmpblk = 0
var slba_cmp = 0
var pat_recover = 0
var pat_round = 0
var start = Date.now()
//var time_spec = 1
var pcy_cycle = 100000
var zone_size=0
var start_lba=0
var TimeOut_Code = 1
var Success_Code = 0
var Error_Code = -1
var MisCompare_Code = -2
var HWError_Code = -3
var DrvError_Code = -4
var pro_tim=0
function nvme_write(slba, nblk,size)
{
    //console.log(slba,nblk)
    zone_size=size
    var sector_num = 0
    start_lba=  slba  

    q_num = Math.floor(wr_comm_num/Q_DEPTH)%Q_NUM+1
    pat.fill(slba,nblk,wr_res)
    select(0, 0, q_num)
    write_async(slba, nblk, 0, wr_res * SECTOR_SIZE)
    wr_comm_num++ //global variable
    wr_sector_num = wr_sector_num + nblk
    wr_res = wr_res + nblk

    if((wr_comm_num % (Q_NUM * Q_DEPTH)==0) && (wr_comm_num >1))
    {
        
        for (var i = 1;i<=Q_NUM;i++)
            door_bell(i)

        var com_inq = Q_NUM * Q_DEPTH
            
        while (com_inq > 0) {

            var ret = retrieve()
            com_inq -= ret.cnt
            for(var i = 0; i<ret.cnt;i++)
            {
                if(ret.results[i].status){

                    info(sprintf('Error Found! cookie: %010d, result: %d, status: %d, costtime: %04d',ret.results[i].cookie,ret.results[i].result,ret.results[i].status,ret.results[i].costtime))
     
                    return HWError_Code
                }   
            }
        }

        wr_res = 0
        pro_tim=progress(slba,wr_sector_num,zone_size) 
        if(pro_tim==1){
            return 1
        }

    } 
    return Success_Code    
}

/*function pat_recv(slba, nblk)
{
    //if (pat_round + nblk < MAX_BUFFER_SIZE / SECTOR_SIZE)
    //{        
        pat.fill(slba,nblk,pat_round)
        pat_round = pat_round + nblk
        pat_recover = pat_recover + nblk
    //}    
}*/

function nvme_readcmp(slba, nblk,size)
{
    zone_size = size
    start_lba = slba
    /*if(rd_res == 0)
        slba_cmp = slba*/
    q_num = (Math.floor(rd_comm_num/Q_DEPTH)%Q_NUM)+1
    //pat_recv(slba,nblk)
    pat.fill(slba,nblk,rd_res)
    select(0, 0, q_num)
    
    read_async(slba, nblk, 0, (rd_res*SECTOR_SIZE))
    rd_comm_num++ //global variable

    rd_sector_num = rd_sector_num + nblk
    rd_res = rd_res + nblk

    if( rd_comm_num % (Q_NUM * Q_DEPTH)==0 && rd_comm_num >1 )
    {

        for (var i = 1;i<=q_num;i++)
        {            
            door_bell(i)           
        }

        var com_inq = Q_NUM * Q_DEPTH        
        while (com_inq > 0 ) {

            var ret = retrieve()
            com_inq -= ret.cnt
            for(var i = 0; i<ret.cnt;i++)
            {
                if(ret.results[i].status){
                    info(sprintf('Error Found! cookie: %010d, result: %d, status: %d, costtime: %04d',ret.results[i].cookie,ret.results[i].result,ret.results[i].status,ret.results[i].costtime))                    
                    return HWError_Code
                }   
            }                      
        }

        pro_tim=progress(slba,rd_sector_num,size)
        if(pro_tim==1){
            return 1
        }
        
        var r = cmp(rd_res*SECTOR_SIZE,0,0)

        if(r)
        {
            send_msg =  "Failed sector_no = "+Math.floor(r/SECTOR_SIZE)+'offset = '+r%SECTOR_SIZE 
            info(send_msg)
            var error_no =Math.floor(r/SECTOR_SIZE)
            console.log('error parameter:',error_no)
            logs.dump_register()
            logs.dump_buff(r)
            out.output_offset(w_buff,error_no,2,2)
            out.output_offset(r_buff,error_no,2,2)
            console.log('================Failed=================',r)
     
            return MisCompare_Code
        }
  
        rd_res = 0
        pat_round = 0         
    }   
   
    return Success_Code
}
function nvme_read(slba, nblk)
{
    
    if(rd_res == 0)
        slba_cmp = slba
    q_num = (Math.floor(rd_comm_num/Q_DEPTH)%Q_NUM)+1

    select(0, 0, q_num)
    
    read_async(slba, nblk, 0, (rd_res*SECTOR_SIZE))
    rd_comm_num++ //global variable
   
    rd_sector_num = rd_sector_num + nblk
    rd_res = rd_res + nblk

    if(rd_comm_num % (Q_NUM * Q_DEPTH)==0 && rd_comm_num >1 )
    {
       //console.log('rd_comm_num:%d,rd_res:%d',rd_comm_num,rd_res)
        for (var i = 1;i<=q_num;i++)
        {            
            door_bell(i)           
        }

        var com_inq = Q_NUM * Q_DEPTH
        
        while (com_inq > 0 ) {

            var ret = retrieve()
            com_inq -= ret.cnt
            for(var i = 0; i<ret.cnt;i++)
            {
                if(ret.results[i].status){

                    info(sprintf('Error Found! cookie: %010d, result: %d, status: %d, costtime: %04d',ret.results[i].cookie,ret.results[i].result,ret.results[i].status,ret.results[i].costtime))
                    
                }   
            }
                     
        }
        rd_res = 0        
    }
}

var comm_retrieved =0
var rcom_retrieved =0
function all_bell()
{

    for (var i = 1;i <= q_num;i++)
    {        
        door_bell(i)       
    }
    var ret = retrieve()
    comm_retrieved = comm_retrieved + ret.cnt

    while(ret.cnt)
    {
        ret = retrieve()
        comm_retrieved = comm_retrieved + ret.cnt

        for(var i = 0; i<ret.cnt;i++)
        {
            if(ret.results[i].status){
                info(sprintf('Error Found! cookie: %010d, result: %d, status: %d, costtime: %04d',ret.results[i].cookie,ret.results[i].result,ret.results[i].status,ret.results[i].costtime))                
                return HWError_Code
            }   
        }
    }
    wr_comm_num=0
    console.log("write.door_bell")
    if(method==2){
        start_lba=zone_size+zone_offset
    }
    pro_tim = progress(start_lba,wr_sector_num,zone_size)

    if(pro_tim==1){
        return 1
    }

    percentage=0
    wr_sector_num=0
    wr_res=0
    return Success_Code 

}

function all_bell_cmp()
{
    
    for (var i = 1;i<=q_num;i++)
    {        
        door_bell(i)       
    }
    var ret = retrieve()
    rcom_retrieved = rcom_retrieved + ret.cnt

    while(ret.cnt)
    {
        ret = retrieve()
        rcom_retrieved = rcom_retrieved + ret.cnt

        for(var i = 0; i<ret.cnt;i++)
        {
            if(ret.results[i].status){
                info(sprintf('Error Found! cookie: %010d, result: %d, status: %d, costtime: %04d',ret.results[i].cookie,ret.results[i].result,ret.results[i].status,ret.results[i].costtime))                
                return HWError_Code
            }   
        }
    }
    
    var r = cmp(rd_res*SECTOR_SIZE,0,0)
    if(r)
    {
        send_msg =  "Failed sector_no = "+Math.floor(r/SECTOR_SIZE)+'offset = '+r%SECTOR_SIZE 
        info(send_msg)
        var error_no =Math.floor(r/SECTOR_SIZE)
        console.log('error parameter:',error_no)
        out.output_offset(w_buff,error_no,1,2)
        out.output_offset(r_buff,error_no,1,2)
        logs.dump_register()
        logs.dump_buff(r)
        console.log('================Failed=================',r)
        return MisCompare_Code
    }
    console.log("compare.door_bell")
    if(method==2){
        start_lba=zone_size+zone_offset
    }
    pro_tim = progress(start_lba,rd_sector_num,zone_size)
    if(pro_tim==1){
        return 1
    }

    percentage=0
    
    rd_comm_num=0
    rd_res = 0
    pat_round = 0
    rd_sector_num=0
    return Success_Code

}
function set_timespec(timelimit)
{
    time_spec = timelimit;

}
function res_cmp()
{
    if (rd_res>0)
    {
        pat.fill(slba_cmp ,rd_res)
        output_4byte(w_buff,rd_res)
        output_4byte(r_buff,rd_res)
        var r =cmp(rd_res,0,0)
        console.log(r)
    }
}
function get_writecount()
{
    return [wr_comm_num,wr_sector_num]
}
function get_readcount()
{
    return [rd_comm_num,rd_sector_num]
}

function lba_verify(slba, nblk)
{

    for(var i = 0; i < nblk; i++)
    {
        var lba_inrbuff = int53.readUInt64BE(r_buff,0)
        if(lba_inrbuff != slba + i)
        {
            return i+1
        }
    }
    return 0   
}
function powercycle(){

    power_off("/home/liuhao/pcie_pa -p /dev/sdb off")
    cls()

    var p_on=power_on("/home/liuhao/pcie_pa -p /dev/sdb on")

    if(p_on[0]==0)
        info("pci device is ok")
    else
        info("the pci device is not ok")
    if(p_on[1]==0)
        info("nvme disk is ok")
    else
        info("nvme disk is not ok")
    scan()

}
function activate_feature(fid)
{
    //info(sprintf('fid :%d activatd',fid));
    switch(fid)
    {

        case 1:
            //info(sprintf('fid :%d activatd',fid));
        break;
    }
}

var step_percent = 1
var percentage = 0
var method=0
var zone_offset=0
function wh_prog(zone_off,meth){
    method = meth
    zone_offset = zone_off
}
function progress(slba,nblk,total_blk)
{
    
    var time_passed = (Date.now() - start) / 1000
    var time_passed_str = Math.floor(time_passed/3600)+":"+Math.floor((time_passed%3600)/60)+":"+((time_passed%3600)%60).toFixed(2)+" "

    var time_left = time_spec * 3600 - (Date.now() - start) / 1000

    if (time_left<0)
        time_left = 0
    else
        time_left = Math.floor(time_left/3600)+":"+Math.floor((time_left%3600)/60)+":"+((time_left%3600)%60).toFixed(2)+" "

    if(method==1 || method==3 || method== 5 || method== 6 || method==7 || method==8 || method==9){

        //console.log('nblk :%d,total_blk :%d,percentage :%d',nblk,total_blk,percentage)
        if((nblk)*100/(total_blk) >= percentage)
        {      
            var send_msg = " slba: "+fmt("%010d",slba) + fmt("    count_lba: %03d",nblk)+',    ' + fmt("%03.3f",(nblk)*100/(total_blk))+ "% done    " + time_passed_str + " passed    " + time_left+" left\n"
            info( 'msg:' + send_msg )
            out.output_info(send_msg+'\n')
            percentage = percentage + step_percent;
        } 
    }
    else if(method==2){
      
        if(slba >= (zone_offset + percentage * total_blk /100)){
            
            send_msg = " slba: "+fmt("%010d",slba) + fmt("    count_lba: %03d",nblk) +'    '+((slba-zone_offset)*100/total_blk).toFixed(3)+ "% done,    " + time_passed_str + " passed    " + time_left +" left\n"
            info( 'msg:' + send_msg )
            out.output_info(send_msg+'\n')
            percentage = percentage + step_percent;

        }
    }
    else if(method==4){

        if(slba <= (zone_offset + total_blk - percentage * total_blk /100)){

            send_msg = " slba: "+fmt("%010d",slba) + fmt("    count_lba: %03d",nblk) +'    '+((total_blk+zone_offset-slba)*100/total_blk).toFixed(3)+ "% done,    " + time_passed_str + " passed    " + time_left +" left\n"
            info( 'msg:' + send_msg )
            out.output_info(send_msg+'\n')
            percentage = percentage + step_percent;

        }
    }
    
    if(percentage > 100)
    {
        percentage = 100      
    }

    if (time_left==0) return TimeOut_Code
  
}

if (require.main === module) 
{
    var int53 = require('int53')
    pat.fill(0x1234567890,1)
    var lba_inwbuff = int53.readUInt64BE(w_buff,0) 
    info(sprintf("lba in w_buff:%x",lba_inwbuff))   
    progress(0x12343,10,100)
}
exports.nvme_write = nvme_write
exports.nvme_read = nvme_read
exports.nvme_readcmp = nvme_readcmp
exports.all_bell = all_bell
exports.get_writecount = get_writecount
exports.get_readcount = get_readcount
exports.res_cmp = res_cmp
exports.all_bell_cmp = all_bell_cmp
exports.wh_prog = wh_prog
exports.set_timespec=set_timespec