utils = require('../utils.js')

function semi_seq(slba, nblk,size) {
    
    function tfor(cb,fin) {

        var zonesize = size
        var start = slba
        var block_num = 0
        var total_blks =0
    
        var seed = utils.seed
        utils.set_rand_seed(seed)

        for (; start < slba + zonesize; start += block_num*2)
        {
            
            block_num = utils.get_write_block(nblk)
            if (start+block_num>slba + zonesize)break
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
            ret = cb(start, zonesize+slba - start,zonesize)
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
        meth(slba,2)
    }

    return {forEach:tfor, meThod:method}
}
exports.semi_seq=semi_seq
if (require.main === module) {
    //for(var j=0;j<6;j++){
        var it = semi_seq(0, 'all',781422768)
        it.forEach(console.log)
    //}
    

}
