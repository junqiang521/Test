utils = require('../utils.js')

function semi_back(slba, nblk,size) {
		
	function tfor(cb,fin) {
        var zonesize = size
        var seed = utils.seed
        utils.set_rand_seed(seed)
        var block_num = utils.get_write_block(nblk)
        var start = slba + size -block_num
		for (; start >= slba; start -= 2*block_num)
		{
			ret = cb(start, block_num,zonesize)
			if(ret==-3){
                info("write fail")
                return -3
            }
            else if(ret==-2){
                info("Miscompare")
                return -2
            }
            else if(ret==1){
                info("timeout")
                return 1
            }
			block_num = utils.get_write_block(nblk)	
		}
		//console.log('block_num:%d,start:%d',block_num,start)	
		if (start < slba)
        {   
           // console.log(start,block_num)
            if(start < 0){
                var red=start+2*block_num-slba
                if(red>0){
                   if(red>256)
                    red=256
                
                    ret = cb(slba,red ,zonesize) 
                }
                
                if(ret==-3){
                    info("write fail")
                    return -3
                }
                else if(ret==-2){
                    info("Miscompare")
                    return -2
                }
                else if(ret==1){
                    info("timeout")
                    return 1
                }
            }

            else //if ((block_num-slba + start + block_num )>0)
            {
                //console.log("11111",start,block_num)
                var red=start+2*block_num-slba
                if(red>0){
                    if(red>256)
                    red=256
                    ret = cb(slba, red,zonesize)
                }
                
            	if(ret==-3){
                    info("write fail")
                    return -3
	            }
	            else if(ret==-2){
	                info("Miscompare")
	                return -2
	            }
	            else if(ret==1){
	                info("timeout")
	                return 1
	            }
            }            	            
        }
        
        if (fin != undefined){
            ret = fin()
            if(ret==-3){
                return -3
            }
            else if(ret==-2){
                return -2
            }
            else if(ret==1){
                info("timeout")
                return 1
            }
        }
    //console.log("total_blks:%d",total_blks)    
		return 0			
    }
	function method(meth) {
        meth(slba,4)
    }

	return {forEach:tfor, meThod:method}
}
exports.semi_back = semi_back
if (require.main === module) {

    var it = semi_back(5786107, '128k',15635085)
    it.forEach(console.log)

}