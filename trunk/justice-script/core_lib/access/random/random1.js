utils = require('../utils.js')
//var interface  = require('../cmd_interface.js')

function random(slba, nblk,size) {

    var BLK_NUM = nblk
	var PART_NUM = 8
    var bord_addr = new Array()
    var temp_size = size / PART_NUM
    var pat_buf1 = new Array()
    var wri_buf1 = new Array()
    var part_size = Math.floor(size/PART_NUM)
    var part_comm = Math.ceil(part_size/BLK_NUM)
    for(var i = 1 ;i<= PART_NUM;i++)
    {
        bord_addr.push(i*part_size+slba)
    }
    //console.log(bord_addr)    
    var pres_size = size-part_size*PART_NUM

    //console.log('part size =',part_size,pres_size)
    utils.set_rand_seed(11)
    for (var i = 0;i<part_comm;i++){
        pat_buf1[i] =i 
    }
    
    for (var i = 0;i<part_comm;i++){        
        var k = utils.get_random(i, part_comm-1)
        wri_buf1[i]=pat_buf1[k]
        pat_buf1[k]=pat_buf1[i]                   
    }
    
	function tfor(cb, fin) {

        var zonesize = size
        var start //= slba
        var block_num = 0
        var total_blks = 0

		for (var i = 0 ; total_blks <size; i++)
		{
			start = slba + wri_buf1[i%part_comm] * BLK_NUM + Math.floor(i/part_comm) * part_size
            //console.log(i,i%part_comm,wri_buf1[i%part_comm],Math.floor(i/part_comm),part_size,Math.floor(i/part_comm) * part_size)
            block_num = nblk//utils.get_write_block(nblk)
            //if (start+block_num>slba + zonesize) break

            if ((start+block_num) > bord_addr[Math.floor(i/part_comm)])
            {
                var  res_blk = bord_addr[Math.floor(i/part_comm)]-start
                ret = cb(start, res_blk,zonesize)
                if(ret==-3){
                    info("write fail")
                    return -3
                }
                else if(ret==-2){
                    info("Miscompare")
                    return -2
                }
                else if(ret==1){
                    info("timeout")
                    return 1
                }
                total_blks = total_blks + res_blk
            }
            else
            {
                ret = cb(start,BLK_NUM,zonesize)
                if(ret==-3){
                    info("write fail")
                    return -3
                }
                else if(ret==-2){
                    info("Miscompare")
                    return -2
                }
                else if(ret==1){
                    info("timeout")
                    return 1
                }
                total_blks = total_blks + block_num
            }            
            
		}
        start = wri_buf1[i]
		//console.log('block_num:%d,start:%d',block_num,start)	
		if (start < slba+zonesize)
        {
			ret = cb(start, zonesize+slba - start,zonesize)
            if(ret==-3){
                info("write fail")
                return -3
            }
            else if(ret==-2){
                info("Miscompare")
                return -2
            }
            else if(ret==1){
                info("timeout")
                return 1
            }
            total_blks = total_blks + zonesize+slba - start
        }
        if (fin != undefined){
            ret = fin()
            if(ret==-3){
                return -3
            }
            if(ret==-2){
                return -2
            }
            else if(ret==1){
                info("timeout")
                return 1
            }
        }
        return 0
    //console.log("total_blks:%d",total_blks)    			
	}
    function abc(me) {
        me(slba,5)
    }

    return {forEach:tfor, aBc:abc}
}
exports.random=random
if (require.main === module) {
    
    var it = random(10, 8,748)
    function tfin()
    {
        console.log("finished.")
    }
    //it.forEach(interface.nvme_write, tfin)
    //it.aBc()
    it.forEach(console.log, tfin)
    //it.aBc()
    
}