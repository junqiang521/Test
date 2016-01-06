var interface  = require('../cmd_interface.js')
utils = require('../utils.js')

function random(slba, nblk,size) {
	
	function tfor(cb, fin) {

        var seed = utils.seed
        utils.set_rand_seed(seed)
        var zonesize = size
        //var start1 = Math.floor(rand(0,(size)))+slba 
        var start1 = utils.get_random(0, size)+slba
        var start=start1
        var block_num = 0
        var total_blks = 0
        
        //console.log(start1)
		for (; start < slba + zonesize; start += block_num)
		{
			//console.log(start,zonesize+slba-start1)
            block_num = utils.get_write_block(nblk)
            if (start+block_num>slba + zonesize) break
            ret = cb(start, block_num,zonesize+slba-start1)
            //console.log(ret)
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
		if (start < slba + zonesize)
        {
			ret =cb(start, zonesize+slba - start,zonesize+slba-start1)
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

exports.random=random
if (require.main === module) {
      
    var it = random(10, 'small',256)
    function tfin()
    {
        console.log("finished.")
    }
    it.forEach(console.log, tfin)
    //it.aBc()
    
}