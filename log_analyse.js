var fmt = require('printf')
var fs = require('fs');
var lineReader = require('line-reader');
var moment = require("moment");
var i=0;
var last_percent = 0
var percent_array = new Array();
var time_array = new Array();
var path =''
var cur_dir = process.argv[1]
var urlChunks = cur_dir.split('/');
for(var i = 0 ;i < urlChunks.length - 2;i++ )
{

	path = path +  urlChunks[i] +'/';
}
path = path + 'info/'
var dirList = fs.readdirSync(path);
console.log(dirList)
console.log('   Time \t\t Time_intv percetage \t perce_intv\n')

dirList.forEach(function(fileName){
	console.log(fileName)
	var name_arry = fileName.split('.')
	if(name_arry[1]=="jcmdll")
	{
		lineReader.eachLine(path+fileName, function(line, last){

			i++;
			var str = line
			var pos1 = str.search(/start_lba/)
			var pos2 = str.search(/sec_num/)
			var str_temp = str.substr(pos1,pos2-pos1);
			str_temp = str.replace(/\s+/ig,'')
			str_temp = str.match(/\d{10}/)
			//console.log(str_temp[0])
			str_temp = str.match(/\d{1,}\.\d{1,}/g)
			
			if(str_temp){

				if(last_percent >Number(str_temp[0])){

					output_array(percent_array,time_array)
					console.log("last len:%d,New Method",percent_array.length-1)
					console.log('   Time \t\t Time_intv percetage \t perce_intv\n')
					percent_array=[]
					time_array = []	
				}
				percent_array.push(str_temp[0])
				time_array.push(line.substr(0,20))
				last_percent = 	Number(str_temp[0])

				//console.log("linenumber:"+i+'\t'+str_temp)
			}
			if(last){

				output_array(percent_array,time_array)

			}
		})
	}

	
})

function output_array(array_a,array_b)
{
	var len = array_a.length;

	for(var j=0;j< len;j++){
		var temp = array_a[j]
		var time_str = array_b[j]
		if(j)
		console.log(time_str+"\t",(moment(array_b[j]) -moment(array_b[j-1]))/1000,'\t',temp,'\t\t',(array_a[j] -array_a[j-1]).toFixed(2));
		else
		console.log(time_str+"\t",'NA\t',temp,'\t\t','NA');	
	}
}