utils = require('../utils.js')

function butterfly2(slba, nblk,size) {
	
	function tfor(cb, fin) {
        var seed = utils.seed
        utils.set_rand_seed(seed)
        var zonesize = size
        var start = slba
        var block_num = 0
        var total_blks = 0
        var mid = slba + Math.floor(size/2)		
        block_num = utils.get_write_block(nblk)
        //console.log(mid)
        for (start = mid - block_num; start > slba; start -= block_num)
        {            
            //if (start-block_num < slba) break
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
            total_blks = total_blks + block_num
            block_num = utils.get_write_block(nblk)
        }
        //console.log('block_num:%d,start:%d',block_num,start)  
        if (start - block_num < slba)
        {

            var res_blk = block_num - slba + start
            ret = cb(slba, res_blk,zonesize)
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
            total_blks = total_blks + res_blk
        }
        //============================================================
        for (start = mid; start < slba + size; start += block_num)
        {   
                
            block_num = utils.get_write_block(nblk)
            if (start+block_num>slba + size) break
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
            total_blks = total_blks + block_num
        }
        //console.log('block_num:%d,start:%d',block_num,start)  
        if (start+block_num>slba + size)
        {
            var res_blk = slba + size - start
            if(res_blk)
            {
                ret = cb(start, res_blk,zonesize)
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
                total_blks = total_blks + res_blk
            }
            
        }
        //console.log('total_blks',total_blks)
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
    //console.log("total_blks:%d",total_blks)    			
	}
    function method(meth) {
        meth(slba,7)
    }
	return {forEach:tfor, meThod:method}
}
exports.butterfly2=butterfly2
if (require.main === module) {
       
    var it = butterfly2(5, 'small',256)
    function tfin()
    {
        console.log("finished.")
    }
    it.forEach(console.log, tfin)
    //it.aBc()   
}