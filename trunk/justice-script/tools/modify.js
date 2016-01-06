var fs = require('fs');
var ini = require('multilevel-ini');
var ini_path='';
var ini_file='';
var ini_pa = ''
var ini_p = ''
var cur_dir = process.argv[1]
var urlChunks = cur_dir.split('/');

for(var i = 0 ;i < urlChunks.length - 2;i++ )
{
    ini_path = ini_path +  urlChunks[i] +'/';
}

var dirList = fs.readdirSync(ini_path);

dirList.forEach(function(fileName){
  
    if(fileName=="core_lib"){

        ini_pa = ini_path+"core_lib"+ "/"
        var dir = fs.readdirSync(ini_pa)
        dir.forEach(function(fileNa){
           if(fileNa=="c_accessmethod_async.js"){
              
                var data=fs.readFileSync(ini_pa+"c_accessmethod_async.js","utf-8")
                fs.writeFileSync(ini_pa+"c_accessmethod_async.js",data.toString().replace('c_pattern.js',"c_pattern.jclib"))
                var dat=fs.readFileSync(ini_pa+"c_accessmethod_async.js","utf-8")
                fs.writeFileSync(ini_pa+"c_accessmethod_async.js",dat.toString().replace('c_logs.js',"c_logs.jclib"))
            }
        })

        dir.forEach(function(fileName){      

            var arr = fileName.split('.')
            if(arr[1]=='js'){
                var  bb = arr[0]+".js"
                var aa = arr[0]+".jclib"
                console.log(aa)
                fs.renameSync(ini_pa+bb,ini_pa+aa,function(err){
                    if(err){
                      console.log(err)
                    }
                    else
                    console.log("done")
                })
            }

        })
    }
    else if(fileName=="core_template") {

        ini_p = ini_path+"core_template"+ "/"
        var dir = fs.readdirSync(ini_p)
        dir.forEach(function(fileName){

            if(fileName=="core_template.js"){

                var data=fs.readFileSync(ini_p+"core_template.js","utf-8")

                fs.writeFileSync(ini_p+"core_template.js",data.toString().replace('c_zone.js',"c_zone.jclib"))
                var dat=fs.readFileSync(ini_p+"core_template.js","utf-8")
                fs.writeFileSync(ini_p+"core_template.js",dat.toString().replace('c_accessmethod_async.js',"c_accessmethod_async.jclib"))
                var da=fs.readFileSync(ini_p+"core_template.js","utf-8")
                fs.writeFileSync(ini_p+"core_template.js",da.toString().replace('c_logs.js',"c_logs.jclib"))
            }
        })
        dir.forEach(function(fileName){

            var arr = fileName.split('.')
            if(arr[1]=='js'){

                var bb = arr[0]+".js"
                var aa = arr[0]+".jctem"
                fs.renameSync(ini_p+bb,ini_p+aa,function(err){
                  if(err){
                    console.log(err)
                  }
                  else
                  console.log("done")
                })
            }

        })
    }
    else if(fileName=="init"){

        pa = ini_path+"init"+ "/"
        var dir = fs.readdirSync(pa)
        dir.forEach(function(fileName){

            if(fileName=="init.js"){

                var data=fs.readFileSync(pa+"init.js","utf-8")
                fs.writeFileSync(pa+"init.js",data.toString().replace('c_logs.js',"c_logs.jclib"))
            }
        })
    }
    else if(fileName=="coretest"){
        p = ini_path+"coretest"+ "/"
        var dir = fs.readdirSync(p)
        dir.forEach(function(fileName){
      
            var data=fs.readFileSync(p+fileName,"utf-8")
            fs.writeFileSync(p+fileName,data.toString().replace('c_logs.js',"c_logs.jclib"))
            var dat=fs.readFileSync(p+fileName,"utf-8")
            fs.writeFileSync(p+fileName,dat.toString().replace('c_pattern.js',"c_pattern.jclib"))
            var da=fs.readFileSync(p+fileName,"utf-8")
            fs.writeFileSync(p+fileName,da.toString().replace('core_template.js',"core_template.jctem"))
            //console.log("ok")
        })
    }
    else if(fileName=="command"){

        p = ini_path+"command"+ "/"
        var dir = fs.readdirSync(p)
        dir.forEach(function(fileName){

            var data=fs.readFileSync(p+fileName,"utf-8")
            fs.writeFileSync(p+fileName,data.toString().replace('c_logs.js',"c_logs.jclib"))
            var dat=fs.readFileSync(p+fileName,"utf-8")
            fs.writeFileSync(p+fileName,dat.toString().replace('c_pattern.js',"c_pattern.jclib"))
            var da=fs.readFileSync(p+fileName,"utf-8")
            fs.writeFileSync(p+fileName,da.toString().replace('core_template.js',"core_template.jctem"))
        });


    }
});

//var data=fs.readFileSync(ini_file,"utf-8")
