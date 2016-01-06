var Random = require("random-js");
var mt_write = Random.engines.mt19937();
var seed = Number(Date.now())%0xffff
var random1 = new Random(mt_write.seed(seed))

function set_rand_seed(seed)
{
    random1 = new Random(mt_write.seed(seed))
}
function get_write_block(wr_size_str){

    var wr_size = 8

    if(isNaN(wr_size_str))
    {

        if( wr_size_str == 'small'){
        
            wr_size =  random1.integer(1,8)
        }
        else if(wr_size_str == 'medium'){
            
            wr_size =  random1.integer(8,64)
        }
        else if(wr_size_str == 'large'){
            
            wr_size =  random1.integer(64,256)
        }
        else if(wr_size_str == 'all'){

            wr_size = random1.integer(1,256)
        }
        else if(wr_size_str == '512b'){
            wr_size = 1
        }
        else if(wr_size_str == '4k'){
            wr_size = 8
        }
        else if(wr_size_str == '8k'){
            wr_size = 16
        }
        else if(wr_size_str == '16k'){
            wr_size = 32
        }
        else if(wr_size_str == '32k'){
            wr_size = 64
        }
        else if(wr_size_str == '64k'){
            wr_size = 128
        }
        else if(wr_size_str == '128k'){
            wr_size = 256
        }
        else
        {
            wr_size = 8
        }  
    }
    else
    {
        wr_size = Number(wr_size_str)
    }
    
    return wr_size
}
function get_random(start,end)
{
    var value = random1.integer(start,end)
    return value
}
exports.set_rand_seed=set_rand_seed
exports.get_write_block=get_write_block
exports.get_random = get_random
exports.seed = seed
if (require.main === module) {
      
    console.log(get_write_block('8k'))    
}