utils = require('../utils.js')

function sequential(slba, nblk,size) {
	
	function tfor(cb, fin) {

        var zonesize = size
        var start = slba
        var block_num = 0
        var total_blks = 0
        var seed = utils.seed
        utils.set_rand_seed(seed)
        //console.log(seed)
		for (; start < slba + zonesize; start += block_num)
		{
			
            block_num = utils.get_write_block(nblk)
            if (start+block_num>slba + zonesize) break
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
		if (start < slba+zonesize)
        {
			ret =cb(start, zonesize+slba - start,zonesize)
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
            
            total_blks = total_blks + zonesize+slba - start
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
        meth(slba,1)
    }

	return {forEach:tfor, meThod:method}
}

exports.sequential=sequential
if (require.main === module) {
      
    var it = sequential(10, 'small',70)
    function tfin()
    {
        console.log("finished.")
    }
    it.forEach(console.log, tfin)
    //it.aBc()
    
}