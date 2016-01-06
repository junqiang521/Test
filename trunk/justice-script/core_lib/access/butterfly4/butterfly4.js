utils = require('../utils.js')

function butterfly4(slba, nblk,size) {
    
    function tfor(cb, fin) {
        
        var seed = utils.seed
        utils.set_rand_seed(seed)
        var zonesize = size
        var block_num = 0
        var total_blks = 0          
        var mid = slba + Math.floor(size/2) 
        block_num = utils.get_write_block(nblk)
        var start = slba
        var end = slba+size-block_num
        //console.log('block_num:%d,start:%d,mid:%d,end:%d',block_num,start,mid,end) 
        for (; start +block_num<= mid,end >= mid;  ) 
        {
            
            //block_num = utils.get_write_block(nblk)
            if(start+block_num>mid) break;
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
            start += block_num,
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

            block_num = utils.get_write_block(nblk)
            end -= block_num

        }
        //console.log('block_num:%d,start:%d,mid:%d,end:%d,total_blks:%d',block_num,start,mid,end,total_blks) 
        if (start +block_num> mid)
        {
            var res_blk = mid - start
            if(res_blk>0)
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
            /*  cb(end, block_num)
            total_blks = total_blks + block_num*/
        }
        //console.log('block_num:%d,start:%d,mid:%d,end:%d,total_blks:%d',block_num,start,mid,end,total_blks) 
        if (end < mid)
        {
            //console.log('Line 41',start ,block_num)
            var res_blk = end+block_num-mid
            //console.log("res_blk2",res_blk,block_num)
            if(res_blk>0)
            {
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
            
        }
        if (end == mid)
        {
            
            //console.log("res_blk3",res_blk,block_num)
            
            {
                ret = cb(mid, block_num,zonesize)
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
        meth(slba,9)
    }
    return {forEach:tfor, meThod:method}
}
exports.butterfly4=butterfly4
if (require.main === module) {    
    var it = butterfly4(10, '128k',65535)
    function tfin()
    {
        console.log("finished.")
    }
    it.forEach(console.log, tfin)
    //it.aBc()    
}