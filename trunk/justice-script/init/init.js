require('../../justice-cmd/command/nvme-cmd.js')
testlogs = require('../core_lib/c_logs.jclib')
var ini = require('multilevel-ini');
//info('//========Reading config.ini.....')
var fs = require('fs');
var ini_path='';
var cur_dir = process.argv[1]
var path = '';
var fmt = require('printf')
var total_lba
var sector_size 
var capacity
var Serial
var Model 
var FirmWare 
var VID 
var device = 0
var cmd_timeout=30000
function init(filename,device_no)
{
	if (fs.existsSync(filename) )
	{
		var result = ini.getSync(filename);
		//device = Number(result['device']['device_no']) 
		cmd_timeout = Number(result['test']['cmdtimeout']) 

		result['core']['pattern']['random_seed'] = Date.now() 
		ini.setSync(result,filename)
	}
	
	set_timeout(cmd_timeout)
	device = device_no
	if (!isNaN(device)){

		select(device,0,0);
	}
	scan()
	
	//get namespace number
	var id_ctrl = identify(1)
	var mdts=id_ctrl.mdts
	 Serial  =  String.fromCharCode.apply(null, id_ctrl.sn);
	var message = "Serial number : " +Serial;
	//info(message)
	//info("Name space number is %d",id_ctrl.nn)
    //info("Name space number is: "+id_ctrl.nn)
    Model  =  String.fromCharCode.apply(null, id_ctrl.mn);
    message = "Model : "  + Model;
    //info(message) 
     FirmWare  =  String.fromCharCode.apply(null,id_ctrl.fr);
    message = "FirmWare : "  + FirmWare;
	//info(message) 

     VID = fmt('0x%04x',(id_ctrl.vid))
    message = "VID: "+ VID
    //info(message)
     nn = id_ctrl.nn
	//get maxlba
	var id_ns = identify(0)
	var max_lba = id_ns.ncap
	var nlbaf = id_ns.nlbaf
	total_lba = id_ns.nsze
	 nsze = id_ns.nsze
	 var mc=id_ns.mc
	//var max_lba = 100000
	var send_msg =  "max_lba  =  " + id_ns.ncap
	//info(send_msg)
	//info( 'msg:' + send_msg  )
	 sector_size = 1<<id_ns.lbaf[id_ns.flbas&0x0f].ds
	 capacity =  Math.floor(max_lba*sector_size/1000000000)
	send_msg =  "Capacity  =  " + Math.floor(max_lba*sector_size/1000000000) +" GiB " + Math.floor(max_lba*sector_size/(1024*1024*1024)) + " GB " +' Sector size = ' + (1<<id_ns.lbaf[id_ns.flbas].ds).toString(10) + ' Bytes'
	//info(send_msg)

	exports.total_lba =total_lba
	exports.sector_size = sector_size
	exports.capacity = capacity
	exports.Serial = Serial
	exports.Model  = Model
	exports.FirmWare = FirmWare
	exports.VID = VID
	exports.device = device
	exports.nn = nn
	exports.nsze = nsze
	exports.mdts = mdts
	exports.nlbaf =nlbaf
	exports.mc=mc
}
	
exports.init = init