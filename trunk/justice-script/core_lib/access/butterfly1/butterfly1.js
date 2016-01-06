utils = require('../utils.js')

function butterfly1(slba, nblk,size) {
	
	function tfor(cb, fin) {

        var zonesize = size
        var start = slba
        var block_num = 0
        var total_blks = 0
        var mid = slba + Math.floor(size/2)
        var seed = utils.seed
        utils.set_rand_seed(seed)
        //block_num = utils.get_write_block(nblk)
		for (; start < mid; start += block_num)
		{
        	block_num = utils.get_write_block(nblk)		           
            if (start+block_num>=mid) break
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
		if (start +block_num>=mid)
        {
			var res_blk = mid - start
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
        //=======================back ward====================
        block_num = utils.get_write_block(nblk)
        for (start = slba + size - block_num; start >= mid; start -= block_num)
        {            
            if (start< mid) break
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
        if (start <mid && (start+block_num)!=mid)
        {
            //console.log('Line 41',start ,block_num)
            var res_blk = start+block_num- mid 
            ret = cb(mid, res_blk,zonesize)
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
        meth(slba,6)
    }

	return {forEach:tfor, meThod:method}
}
exports.butterfly1=butterfly1
if (require.main === module) {
    
    var it = butterfly1(10, '128k',5000)
    function tfin()
    {
        console.log("finished.")
    }
    it.forEach(console.log, tfin)
    //it.aBc()   
}