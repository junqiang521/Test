var fs = require('fs');
var core_path='';
var coretest_file='';
var cur_dir = process.argv[1]
var urlChunks = cur_dir.split('/');
 for(var i = 0 ;i < urlChunks.length - 2;i++ )
 {

  core_path = core_path +  urlChunks[i] +'/';
 }
 core_path = core_path +'coretest/'

 var dirList = fs.readdirSync(core_path);
 dirList.forEach(function(fileName){
     var arr = fileName.split('.')
     if(arr[0]!="1000"){
         if(arr[1]=='jcore'){
        fs.unlinkSync(core_path+fileName)
     }
     }
     })
var dirList = fs.readdirSync(core_path);
dirList.forEach(function(fileName){
     var arr = fileName.split('.')
     if(arr[1]=='jcore'){

      	coretest_file = core_path + fileName;

     }
        
   });

var data=fs.readFileSync(coretest_file,"utf-8")


var test_type = ['1']//for Core and Focus
var q_num     = [0,2,3]
var qid //
var block_size =[0,1,2,3,4,5,6] // block size
var block_size_str =['all','small','medium','large','512b','4k','128k']
var zone_num  =[0,1,2,3]; //for single and multipler zones 
var acs_meth  =[0,1,2]; //for access method Random Group Full
var acs_meth_str =['mix','grp','all']
var Test_NO = '';

for (var i = 0;i <1;i++){
    for (var j= 0; j < zone_num.length;j++){

      for(var n= 0; n < acs_meth.length;n++){

        for (var k = 0;k <block_size.length;k++){
          Test_ = test_type[i] +zone_num[j]+ acs_meth[n]+ block_size[k]+".jcini";
          Test_NO = test_type[i] +zone_num[j]+ acs_meth[n]+ block_size[k]+'.jcore';
          //console.log(Test_NO)
          fs.writeFile(core_path+Test_NO,data.toString().replace('1000.jcini',Test_))
          

        }

        
      }
    }
}