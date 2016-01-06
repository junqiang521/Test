
utils = require('../utils.js')


function ovlp_seq(slba, nblk,size,loop,skip,space) {
    
    function tfor(cb, fin) {

        var zonesize = size
        var start = slba
        var block_num = 0
        var total_blks = 0

        for (; start < slba + zonesize; start += (block_num+space))
        {
            
            block_num = utils.get_write_block(nblk)
            if (start + block_num > slba + zonesize) break


            for(var i = 0;i < loop;i++)
            {
                if(block_num-skip>0)
                    cb(start + skip * i, block_num-skip*i)
                else
                    cb(start , block_num)
            }
            

            total_blks = total_blks + block_num * loop
        }
        //console.log('block_num:%d,start:%d',block_num,start)  
        if (start < slba+zonesize)
        {
            
            for(var i = 0;i < loop;i++)
            {
                //cb(start, zonesize+slba - start)
                if(zonesize+slba - start-skip>0)
                    cb(start + skip*i, zonesize+slba - start -skip*i)
                else
                    cb(start , zonesize+slba - start)
            }
            total_blks = total_blks + (zonesize+slba - start) * loop
        }
        if (fin != undefined)
            fin()
    //console.log("total_blks:%d",total_blks)    
            
    }
    function abc() {}

    return {forEach:tfor, aBc:abc}
}
exports.ovlp_seq=ovlp_seq
if (require.main === module) {
   
    
    var it = ovlp_seq(10, 'medium',210,2,1,1)
    function tfin()
    {
        console.log("finished.")
    }
    it.forEach(console.log, tfin)
    it.aBc()
    
}