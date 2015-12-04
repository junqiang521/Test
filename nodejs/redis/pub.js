var redis = require("redis");
var client = redis.createClient(6379,'127.0.0.1');
 
try{

    
    client.on(

        "error",

        function(err){

            console.log("err"+err);

            }

 

    );

    client.on('ready',

        function(){

            client.publish('wangbin_test',"test,i am test");
 			client.publish('wangbin_test2',"test, i am test2");

            client.end();

            }

    );

}

catch(e){

        console.log("err:"+e);

}
