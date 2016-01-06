utils = require('../utils.js')

function backward(slba, nblk,size) {
	
	function tfor(cb,fin) {

        var zonesize = size
        utils.set_rand_seed(11)
        var block_num = utils.get_write_block(nblk)
        var start = slba + size -block_num

        var seed = utils.seed
        utils.set_rand_seed(seed)
        
		for (; start >= slba; start -= block_num)
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
            if (block_num-slba + start>0)
            ret = cb(slba, (block_num-slba + start ),zonesize)
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
            //console.log('block_num:%d,start:%d',block_num,start)
        }
        if (fin != undefined){
            ret = fin()
            if(ret==-3){
                return -3
            }
            if(ret==-2){
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
        meth(slba,3)
    }

    return {forEach:tfor, meThod:method}
}
exports.backward = backward
if (require.main === module) {

    var it = backward(10, 'small',60)
    it.forEach(console.log)
}