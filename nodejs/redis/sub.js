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

            client.subscribe('wangbin_test');

            client.subscribe('wangbin_test2');

            //client.end();

            }

    );

    client.on('subscribe',

        function(channel,count){

            console.log("channel:" + channel + ", count:"+count);

            }

    );

    client.on('message',

        function(channel,message){

            console.log("channel:" + channel + ", msg:"+message);

            }

    );

    client.on('unsubscribe',

        function(channel,count){

            console.log("channel:" + channel + ", count:"+count);

            }

    );

}

catch(e){

        console.log("err:"+e);

}
