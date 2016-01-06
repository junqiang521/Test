require('../../justice-cmd/command/nvme-cmd.js')
var fmt = require('printf')
scan()
var ret = get_pci_link()
	//console.log("pci link,cap speed:gen%d,cap width:%dx,current speed:gen%d,current width:%dx",
	//	    ret[0],ret[1],ret[2],ret[3])
	ret = get_driver_version()
	if((ret&0x000000FF) == 0x0A)
	  state = 'Alpha'
	else if ((ret&0x000000FF) == 0x0B)
	  state = 'Beta'
	else 
	  state = 'Release'
	prnt_str = fmt("Driver Version:%d.%d.%d %s\n",ret>>24,(ret>>16)&0x00FF,(ret>>8)&0x0000FF,state)
	//prnt_str = 'Version :\n'

	//prnt_str = prnt_str +"PCIE info:\n"

	prnt_str = prnt_str +"Load driver successful\n"

	info(prnt_str)
	info('end')
	cls();
	