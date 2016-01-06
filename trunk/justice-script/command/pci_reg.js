require('../../justice-cmd/command/nvme-cmd.js')
var sprintf = require("sprintf-js").sprintf
var logs =  require('../output/output.js');
var res = new Array(10);
var res_str="";
scan();
console.log(process.argv);
var command = process.argv[2]//process.env.CMD_TYPE;
var offset = Number(process.argv[3])//process.env.REG_OFFSET;
var value_low = Number(process.argv[4]) //process.env.REG_VALE_LOW;
var value_hi  =  Number(process.argv[5])//process.env.REG_VALE_HI;
var len_dw =  Number(process.argv[6])//process.env.LEN_DW;
var i = 0;
if (len_dw>2 ||len_dw<=0) 
{
	info('len_dw must <=2 and len_dw > 0');
	
}
else
{
	if(command == 'reg_read')
	{
		for(i=0;i<len_dw;i++)
		{
			res[i] = reg_read(offset+i*4);
			res_str = res_str + sprintf("reg_read addr = 0x%04x,value=0x%08x\n",offset+i*4,res[i])
		}
		info(res_str)
		
	}
	else if(command == 'reg_write')
	{
		//nvme_unbind();
		if(len_dw == 1)
		{
			reg_write(offset,value_low);
			res_str = res_str + sprintf("reg_write addr = 0x%04x,value=0x%08x\n",offset,value_low)
			res[0] = reg_read(offset);
			res_str = res_str + sprintf("reg_read addr = 0x%04x,value=0x%08x\n",offset,res[0])
		}
		else
		{
			
			reg_write(offset,value_low);
			reg_write(offset+4,value_hi);
			res_str = res_str + sprintf("reg_write addr = 0x%04x,value=0x%08x\n",offset,value_low)
			res_str = res_str + sprintf("reg_write addr = 0x%04x,value=0x%08x\n",offset+4,value_hi)
			res[0] = reg_read(offset);
			res[1] = reg_read(offset+4);
			res_str = res_str + sprintf("reg_read addr = 0x%04x,value=0x%08x\n",offset,res[0])
			res_str = res_str + sprintf("reg_read addr = 0x%04x,value=0x%08x\n",offset+4,res[1])
		}
		info(res_str)
		//nvme_bind();
	}
	else if(command == 'reg_readall')
	{
		for(i=0;i<16;i++)
		{
			res[i] = reg_read(i*4);
			res_str = res_str + sprintf("read_all addr = 0x%04x,value=0x%08x\n",i*4,res[i])
			info(res_str)
       			res_str='';
		}

		for(i=0;i<8;i++){

			res[i] = reg_read(0x1000+i*4);
			res_str = res_str + sprintf("read_all addr = 0x%04x,value=0x%08x\n",0x1000+i*4,res[i])
			info(res_str)
       			res_str='';
		}
		

	}
}

info('end');
cls();