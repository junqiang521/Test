require('../../justice-cmd/command/nvme-cmd.js')  
var ini = require('multilevel-ini');
var fs =  require('fs');
var cur_dir = process.argv[1]
var ini_path='';
var urlChunks = cur_dir.split('/');
var result

cur_dir = cur_dir - urlChunks[urlChunks.length - 1]
for(var i = 0 ;i < urlChunks.length - 2;i++ )
{

	ini_path = ini_path +  urlChunks[i] +'/';
}
	ini_path = ini_path + 'core_ini/'

info(ini_path);
var dirList = fs.readdirSync(ini_path);

dirList.forEach(function(fileName){
	var arr = fileName.split('.')
	if(arr[1]=='jcini'){
    	info(ini_path + fileName);
     	result= ini.getSync(ini_path + fileName);
     	result['device']['device_no'] = Number(process.argv[2])
     	ini.setSync(result,ini_path + fileName)
     }
        
 });
 
info('end')
cls();
