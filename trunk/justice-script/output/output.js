var log = require('fs');
var fmt = require('printf')
var moment = require("moment");
var os = require('os');

var w_buff = get_write_buffer()
var r_buff = get_read_buffer()

var SECTOR_SIZE = 512
var cur_dir = process.argv[1]
var file_name='';
var file1 = ""
var file2 = ""
var file = ""
 var urlChunks = cur_dir.split('/');
 var testid
var n = urlChunks.length
var testname = urlChunks[n-1]
var test = testname.split('.')
testid = test[0]
//var time =moment().format("YYYY-MM-DD")+"-"+ moment().format("HH:mm:ss")
var time
var sysid=os.hostname()
var pid = process.pid
 for(var i = 0 ;i < urlChunks.length - 2;i++ )
 {

  file_name = file_name +  urlChunks[i] +'/';
 }
// file_name = file_name + 'info/info.txt';
exports.out=function(time){
	file = file_name + 'info/'+testid+"_"+time+"_"+sysid+"_"+pid+".jlog";
}
file1 =  file_name + 'info/'+testid+"_"+time+"_"+sysid+"_"+pid+".wbuff"
file2 =  file_name + 'info/'+testid+"_"+time+"_"+sysid+"_"+pid+".rbuff"


exports.output_4k = function(id_arry,sec_num) {

  var i
  var prtstr=''
  info('--------------- 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\n')
  for (i = 0; i < sec_num*SECTOR_SIZE; i++) {

    prtstr = prtstr + fmt('%02x',id_arry[i]) + " "
    if ((i+1) % 16== 0)  {

	    prtstr = fmt('line:%04x\t',Number(i/16))+ prtstr;
	    info(prtstr);
	    prtstr='';
    }
    //info('---------------------------------------------------------------')
  }
}
exports.output_nline = function(id_arry,sec_num,n) {

  var i
  var prtstr=''
    info('--------------- 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\n')
  for (i = 0; i < sec_num; i++) {
    
    for (var j =0;j< n*16;j++)
    {
    	prtstr = prtstr + fmt('%02x',id_arry[i*SECTOR_SIZE+j]) + " ";
    	if ((j+1) % 16== 0)
    	{
    		prtstr = fmt('line:%04x\t',Number(j/16))+ prtstr;
    		info(prtstr);
	    	prtstr='';
    	}
    }
    info('---------------------------------------------------------------')
   
  }
}
exports.output_offset = function(id_arry,offset,sec_num,n) {

  var i
  var prtstr=''
    info('--------------- 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\n')
  for (i = offset; i < sec_num+offset; i++) {
    
    for (var j =0;j< n*16;j++)
    {
      prtstr = prtstr + fmt('%02x',id_arry[i*SECTOR_SIZE+j]) + " ";
      if ((j+1) % 16== 0)
      {
        prtstr = fmt('line:%04x\t',Number(j/16))+ prtstr;
        info(prtstr);
        prtstr='';
      }
    }
    info('---------------------------------------------------------------')
   
  }
}
exports.output = function(id_arry, addr) {

  var i

  for (i = 0; i < 1 ; i++) {
	info("%d : ",i,
	id_arry[addr + 0 + i * 32].toString(16),id_arry[addr + 1 + i * 32].toString(16),id_arry[addr + 2 + i * 32].toString(16),id_arry[addr + 3 + i * 32].toString(16),
	id_arry[addr + 4 + i * 32].toString(16),id_arry[addr + 5 + i * 32].toString(16),id_arry[addr + 6 + i * 32].toString(16),id_arry[addr + 7 + i * 32].toString(16),
	id_arry[addr + 8 + i * 32].toString(16),id_arry[addr + 9 + i * 32].toString(16),id_arry[addr + 10 + i * 32].toString(16),id_arry[addr + 11 + i * 32].toString(16),
	id_arry[addr + 12 + i * 32].toString(16),id_arry[addr + 13 + i * 32].toString(16),id_arry[addr + 14 + i * 32].toString(16),id_arry[addr + 15 + i * 32].toString(16),
	id_arry[addr + 16 + i * 32].toString(16),id_arry[addr + 17 + i * 32].toString(16),id_arry[addr + 18 + i * 32].toString(16),id_arry[addr + 19 + i * 32].toString(16),
	id_arry[addr + 20 + i * 16].toString(16),id_arry[addr + 21 + i * 32].toString(16),id_arry[addr + 22 + i * 32].toString(16),id_arry[addr + 23 + i * 32].toString(16),
	id_arry[addr + 24 + i * 16].toString(16),id_arry[addr + 25 + i * 32].toString(16),id_arry[addr + 26 + i * 32].toString(16),id_arry[addr + 27 + i * 32].toString(16),
	id_arry[addr + 26 + i * 16].toString(16),id_arry[addr + 29 + i * 32].toString(16),id_arry[addr + 30 + i * 32].toString(16),id_arry[addr + 31 + i * 32].toString(16)
	)
  }
}
exports.output_4byte = function  (id_arry, sector) {
  var i
  
  for (i = 0; i < sector ; i++) {
	  
		info("%d : ",i,
		id_arry[ i * SECTOR_SIZE+4].toString(16),id_arry[ 5 + i * SECTOR_SIZE].toString(16),id_arry[ 6 + i * SECTOR_SIZE].toString(16),id_arry[ 7 + i * SECTOR_SIZE].toString(16))
	
	}
  
}
exports.output_file = function (id_arry, sector) {

	for (i = 0; i < sector ; i++) {

		log.appendFileSync('record.txt',"Line:"+ i + "\t"+id_arry[ i * SECTOR_SIZE].toString(16)+ "\t"+id_arry[ 1 + i * SECTOR_SIZE].toString(16)+ "\t"+id_arry[ 2 + i * SECTOR_SIZE].toString(16)+ "\t"+id_arry[ 3 + i * SECTOR_SIZE].toString(16)+"\n")
	}
}
exports.output_info = function (str) {

	var time =moment().format("MM-DD")+" "+ moment().format("HH:mm:ss")
	
    log.appendFileSync(file,time+"\t "+str)
	
}

