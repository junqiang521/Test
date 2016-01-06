utils = require('../utils.js')
//.............75312468...............

function butterfly3(slba_zone, nblk,size) {
	
	function tfor(cb, fin) {

        var seed = utils.seed
        utils.set_rand_seed(seed)
        var zonesize = size

        var block_num = 0
        var total_blks = 0
        var mid = slba_zone + Math.floor(size/2)		
        block_num = utils.get_write_block(nblk)
        var start = mid -block_num
        var end = mid        
        //console.log("mid",mid,start)
        for (; start >= slba_zone;  )
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
        //console.log('block_num:%d,start:%d,end:%d',block_num,start,end) 
        
        if (start - block_num < slba_zone)
        {
            //console.log('Line 57',start ,block_num)
            var res_blk = block_num - slba_zone + start
            if (res_blk>0)
            ret = cb(slba_zone, res_blk,zonesize)
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
        //console.log('block_num:%d,start:%d,end:%d',block_num,start,end) 
        if (end + block_num >= slba_zone+zonesize)
        {
            //console.log('Line 79',start ,block_num)
            var res_blk =  slba_zone + size - end
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
        else
        {
             //console.log('Line 99',start ,block_num)
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
            //console.log("total_blks:%d",total_blks)   
        }
    // 
        return 0			
	}
    function method(meth) {
        meth(slba_zone,8)
    }

	return {forEach:tfor, meThod:method}
}
exports.butterfly3=butterfly3
if (require.main === module) {
      
    var it = butterfly3(10, '128k',66561)
    function tfin()
    {
        console.log("finished.")
    }
    it.forEach(console.log, tfin)
    //it.aBc()
    
}