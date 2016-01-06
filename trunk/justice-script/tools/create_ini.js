var ini = require('multilevel-ini');
var fs = require('fs');

var ini_path='';
var ini_file='';
var cur_dir = process.argv[1]
var urlChunks = cur_dir.split('/');

 for(var i = 0 ;i < urlChunks.length - 2;i++ )
 {

  ini_path = ini_path +  urlChunks[i] +'/';
 }
 ini_path = ini_path + 'core_ini/'
  var dirList = fs.readdirSync(ini_path);
 dirList.forEach(function(fileName){
     var arr = fileName.split('.')
     if(arr[0]!="1000"){
     	   if(arr[1]=='jcini'){
     	   	//var aa = arr[0]+".jcini"
      	fs.unlinkSync(ini_path+fileName)
     }
     }
  
        
   });
 
 var dirList = fs.readdirSync(ini_path);


dirList.forEach(function(fileName){
     var arr = fileName.split('.')
     if(arr[1]=='jcini'){

      	ini_file = ini_path + fileName;
      	
     }
        
   });

var result = ini.getSync(ini_file );
 
var test_type = ['1']//for Core and Focus
var q_num     = [0,2,3]
var qid //
var fulldisk
var random
var overlap
var no_overlap
//var comm_type = ['A','S']//for ASYNC and SYNC
var block_size =[0,1,2,3,4,5,6] // block size
var block_size_str =['all','small','medium','large','512b','4k','128k']
var zone_num  =[0,1,2,3]; //for single and multipler zones 
var acs_meth  =[0,1,2]; //for access method Random Group Full
var acs_meth_str =['mix','grp','all']
var Test_NO = '';
var start = Date.now()
var Device_No =0;
var time_now = null 
for (var i = 0;i <1;i++){
		for (var j= 0; j < zone_num.length;j++){

			for(var n= 0; n < acs_meth.length;n++){

				for (var k = 0;k <block_size.length;k++){

					Test_NO = test_type[i] +zone_num[j]+ acs_meth[n]+ block_size[k]+'.jcini';
				
					//result['test']['q_num'] = 1<<q_num[qid]
					if (zone_num[j]=='0')
					{
						//result['test']['ismulti'] = 'false'
						result['core']['zone']['num'] = 1
						result['core']['region']['num'] = 1
						result['core']['zone']['type']  = 'full_region'
						result['core']['region']['type'] = 'seq'
						result['core']['zone']['reg_id'] = 0
                        result['test']['Zone'] = "fulldisk"
					}
					else if (zone_num[j]=='1')
					{
						//result['test']['ismulti'] = 'false'
						result['core']['zone']['num'] = 10
						result['core']['region']['num'] = 19
						result['core']['zone']['type']  = 'ran_zone'
						result['core']['region']['type'] = 'seq'
						result['core']['zone']['reg_id'] = 0
                        result['test']['Zone'] = "random"

					}
					else if (zone_num[j]=='2')
					{
						//result['test']['ismulti'] = 'true'
						result['core']['zone']['num'] = 10
						result['core']['region']['num'] = 19
						result['core']['zone']['type']  = 'seq_zone'
						result['core']['region']['type'] = 'over'
						result['core']['zone']['reg_id'] = 8
                        result['test']['Zone'] = "overlap"

					}
					else if (zone_num[j]=='3')
					{
						//result['test']['ismulti'] = 'true'
						result['core']['zone']['num'] = 10
						result['core']['region']['num'] = 19
						result['core']['zone']['type']  = 'seq_zone'
						result['core']['region']['type'] = 'seq'
						result['core']['zone']['reg_id'] = 0
                        result['test']['Zone'] = "no_overlap"
					}

					result['core']['zone']['wr_size'] = block_size_str[k]
					result['core']['zone']['acsmethd'] = acs_meth_str[n]
					//result['test']['timelimit'] = 3
					ini.setSync(result,ini_path+Test_NO);
					//console.log(Test_NO)

				}

				
			}
		}

	
	
}


