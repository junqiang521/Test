utils = require('../utils.js')
//.............75312468...............

function butterfly3(slba, nblk,size) {
	
	function tfor(cb, fin) {
        utils.set_rand_seed(11)
        var zonesize = size

        var block_num = 0
        var total_blks = 0
        var mid = slba + Math.floor(size/2)		
        block_num = utils.get_write_block(nblk)
        var start = mid -block_num
        var end = mid
        
        //console.log("mid",mid,start)
        for (; start - block_num >= slba;  )
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
        //console.log('block_num:%d,start:%d',block_num,start,end)  
        if(start<0){
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
        
            if(end+block_num>=zonesize+slba){
                var res_blk = zonesize+slba -end 
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
      
    var it = butterfly3(10, '128k',51233)
    function tfin()
    {
        console.log("finished.")
    }
    it.forEach(console.log, tfin)
    //it.aBc()
    
}