utils = require('../utils.js')
//.............75312468...............

function butterfly3(slba, nblk,size) {
	
	function tfor(cb, fin) {
        var seed = utils.seed
        utils.set_rand_seed(seed)
        var zonesize = size

        var block_num = 0
        var total_blks = 0
        var mid = slba + Math.floor(size/2)		
        block_num = utils.get_write_block(nblk)
        var start = mid -block_num
        var end = mid
        
        //console.log("mid",mid,start)
        for (; start >= slba;  )
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
            total_blks = total_blks + block_num

            ret = cb(end, block_num,zonesize)
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
            end += block_num
            block_num = utils.get_write_block(nblk)
            start -= block_num          
        }
        console.log('block_num:%d,start:%d,end:%d',block_num,start,end) 
        
        if(start<0){
            var res_blk = block_num - slba + start
            if(res_blk>0)
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
        
            if(end+block_num>=zonesize+slba){
                var res_blk = zonesize+slba -end
                if(res_blk>0) 
                ret = cb(end, res_blk,zonesize)
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
        else if (start - block_num < slba)
        {
            //console.log('Line 40',start ,block_num)
            var res_blk = block_num - slba + start
            if(res_blk>0)
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
        
            if (end + block_num <= slba+zonesize)
            {
                //console.log('Line 41',start ,block_num)
                var res_blk =  slba + size - end
                if(res_blk>0)
                ret = cb(end, res_blk,zonesize)
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
        //============================================================
        
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
	}
    function method(meth) {
        meth(slba,8)
    }

	return {forEach:tfor, meThod:method}
}
exports.butterfly3=butterfly3
if (require.main === module) {
      
    var it = butterfly3(10, '128k',1211)
    function tfin()
    {
        console.log("finished.")
    }
    it.forEach(console.log, tfin)
    //it.aBc()
    
}