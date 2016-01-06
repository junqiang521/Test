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

                Test_NO = test_type[i] +zone_num[j]+ acs_meth[n]+ block_size[k]+'.jcore';
                
                fs.writeFileSync(core_path+Test_NO,data)
              
            }
          
        }
    }
}

out:
for (var i = 0;i <1;i++){

    for (var j= 0; j < zone_num.length;j++){

        for(var n= 0; n < acs_meth.length;n++){

            for (var k = 0;k <block_size.length;k++){

                Test_NO = test_type[i] +zone_num[j]+ acs_meth[n]+ block_size[k]+'.jcore';
                if (zone_num[j]=='0')
                {
                
                }
                else if (zone_num[j]=='1')
                {

                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("num: '1',","num: '10',"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("type: 'seq', num: '1'","type: 'seq', num: '19'"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("full_region","ran_zone"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("fulldisk","random"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")

                }
                else if (zone_num[j]=='2')
                {
                   
                    var data =fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("num: '1',","num: '10',"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("type: 'seq', num: '1'","type: 'seq', num: '19'"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("full_region","seq_zone"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("type: 'seq'","type: 'over'"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("reg_id: '0'","reg_id: '8'"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("fulldisk","overlap"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")

                }
                else if (zone_num[j]=='3')
                {
               
                    var data =fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("num: '1',","num: '10',"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("type: 'seq', num: '1'","type: 'seq', num: '19'"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("full_region","seq_zone"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("type: 'seq'","type: 'over'"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("reg_id: '0'","reg_id: '8'"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")
                    fs.writeFileSync(core_path+Test_NO,data.toString().replace("fulldisk","no_overlap"))
                    var data = fs.readFileSync(core_path+Test_NO,"utf-8")

                }

                
                fs.writeFileSync(core_path+Test_NO,data.toString().replace("wr_size: 'all',","wr_size: '"+block_size_str[k]+"',"))
                
                var data = fs.readFileSync(core_path+Test_NO,"utf-8")
                fs.writeFileSync(core_path+Test_NO,data.toString().replace("acsmethd: 'mix'","acsmethd: '"+acs_meth_str[n]+"'"))
                
            }
          
        }
    }
}
console.log("creat jcore file ok")