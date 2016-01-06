require('../../justice-cmd/command/nvme-cmd.js')
var pat = require('../core_lib/c_pattern.jclib')
var iniat = require('../init/init.js')
var output=require('../output/output.js')
var shell = require('shelljs')
var ref_struct = require('ref-struct')
var ref = require('ref')
var ref_array = require('ref-array')
var fmt = require('printf')
var char_t = ref.types.char
var uint8_t = ref.types.uint8
var uint16_t = ref.types.uint16
var uint32_t = ref.types.uint32
var uint64_t = ref.types.uint64
//scan()
iniat.init()
var w_buff = get_write_buffer()
var r_buff = get_read_buffer()
var IRQ_Str = [
	'MSI-X',
	'MSI',
	'LEGACY']

function  gui_cmd(cmd,para1,para2,para3){
	
	
	if(cmd=='identify'||cmd=='idfy'){

		 
	if(para1==1){
	 var idfy=identify(para1)
	 info("Bytes 01:00  \tVID(PCI Vendor ID):"+fmt('0x%04x',(idfy.vid))+'\n')			 
	
	 info("Bytes 03:02\tSSVID(PCI Subsystem Vendor ID):"+fmt('0x%04x',(idfy.ssvid))+'\n')
	
	
	// Serial number  20 char 

	 var Serial  =  String.fromCharCode.apply(null, idfy.sn);

    message = "Serial number : " +Serial;
    info("Bytes 23:04 \t "+message+'\n')
	 
	 // Model 40 char 
	var Model  =  String.fromCharCode.apply(null, idfy.mn);
	message = "Model : "  + Model;
	info("Bytes 63:24\t "+message+'\n') 
    
	
	// Firmware 8 char 
	var FW  =  String.fromCharCode.apply(null,idfy.fr);

	message = "Firmware ver : " + FW;
    info("Bytes 71:64  \t"+message+'\n')
	

	 info("Byte     72  \tRAB(Recommended Arbitration Burst):"+fmt('0x%02x',(idfy.rab))+'\n') 

	 info("Bytes 75:73  \tIEEE(IEEE OUI Identifier):"+fmt('%02x',(idfy.ieee[0]))+fmt('%02x',(idfy.ieee[1]))+fmt('%02x',(idfy.ieee[2]))+'\n')
	 		 

	 info("Byte     76  \tCMIC(Controller Multi-Path I/O and Namespace sharing Capabilities):"+fmt('0x%02x',(idfy.cmic))+'\n')
	
	 info('\t\tBits 7:3  \treserved')
	 info('\t\tBit 2\tset to 1 , the controller is associated with an SR-IOV Virtual Function')
	 info('\t\t\tset to 0 , the controller is associated with a PCI Function')
	 info('\t\tBit 1\tset to 1 , the NVM subsystem may contain two or more controllers')
	 info('\t\t\tset to 0 , the NVM subsystem contains only a single controller')
	 info('\t\tBit 0\tset to 1 , the NVM subsystem may contain two or more physical PCI Express ports')
	 info('\t\t\tset to 0 , the NVM subsystem contains only a single PCI Express ports'+'\n')
	 info("Byte     77  \tMDTS(Maximum Data Transfer Size):"+fmt('0x%02x',(idfy.mdts))+'-Means '+(1<<idfy.mdts)+' * CAP.MPSMIN bytes \n')
	
     info("Bytes 79:78  \tCNTLID(Controller ID):"+fmt('0x%04x',(idfy.cntlid))+'\n')
	 
	 //info("        Admin Command Set Attributes")
	 info("Bytes 83:80  \tVER(Version):"+fmt('0x%08x',(idfy.ver))+'\n')
	 
	 info("Bytes 87:84 \tRTD3R(RTD3 Resume Latency):"+fmt('0x%08x',(idfy.rtd3r))+'\n')
	
	 info("Bytes 91:88 \tRTD3E(RTD3 Entry Latency):"+fmt('0x%08x',(idfy.rtd3e))+'\n')
	
	 info("Bytes 95:92 \tOAES(Optional Asynchronous Events Supported):"+fmt('0x%08x',(idfy.oaes))+'\n')
	
	 info('\t\tBits 31:9  \treserved')
	 info('\t\tBit 8\tset to 1 , the controller supports sending the Namespace Attribute Changed event')
     info('\t\t\tset to 0 , does not support send the Namespace Attribute Changed event')			 
	 info('\t\tBits 7:0  reserved'+'\n')
	// info("idfy.rsvd96:"+idfy.rsvd96)
	 info('Bytes 239:96 \tReserved'+'\n')
	 
	 info('Bytes 255:240 \tRefer to the NVMe Management Interface Specification for definition'+'\n')
	 
	 info("Bytes 257:256 \tOACS(Optional Admin Command Support):"+fmt('0x%04x',(idfy.oacs))+'\n')
	
	 //info("support firmware activate and firmware download cmd ")
	// info("support format NVM command")
	 info("\t\tBits 15:4  reserved")
	 info("\t\tBit 3\tset to 1 , support the Namespace Management and Namespace Attachment commands")
	 info("\t\t\tset to 0 , does not support the Namespace Management and Namespace Attachment commands")
	 info("\t\tBit 2\tset to 1 , support Firmware Commit and Firmware Image download commands ")
	 info("\t\t\tset to 0 , does not support Firmware Commit and Firmware Image download commands")
	 info("\t\tBit 1\tset to 1 , support Format NVM command")
	 info("\t\t\tset to 0 , does not support Format NVM command")
	 info("\t\tBit 0\tset to 1 , support Security Send and Security Receive commands")
	 info("\t\t\tset to 0 , does not support the Security Send and Security Receive commands\n")
	 info("Byte 258 \tACL(Abort Command Limit):"+fmt('0x%02x',(idfy.acl))+'\n')	
     info('The maximum number of concurrently outstanding Abort commands.This is a 0 based value')			 
	 info("Byte 259 \tAERL(Asynchronous Event Request Limit):"+fmt('0x%02x',(idfy.aerl))+'\n')
     info('The maximum number of concurrently outstanding Asynchronous Event Request commands supported .This is a 0 based value')			 
	 info("Byte 260 \tFRMW(Firmware Updates):"+fmt('0x%02x',(idfy.frmw))+'\n')		
	 info("\t\tBits 7:5  reserved")
	 info("\t\tBit 4\tset to 1,the controller supports firmware activation without a reset")
	 info("\t\t\tset to 0,the controller requires a reset for firmware to be activated")
	 info("\t\tBits 3:1  the number of firmware slots")
	 info("\t\tBit 0\tset to 1 means the first firmware slot(slot 1) is read only")
	 info("\t\t\tset to 0 means the first firmware slot(slot 1) is read/write")
	 info("Byte 261 \tLPA(Log Page Attributes):"+fmt('0x%02x',(idfy.lpa))+'\n')			
	 info("\t\tBits 7:2  reserved")
	 info("\t\tBit 1\tset to 1 , support the Command Effects log page")
	 info("\t\t\tset to 0 , does not support the Command Effects log page")
	 info("\t\tBit 0\tset to 1 , support the SMART/Health information log page on a per namespace basis")
	 info("\t\t\tset to 0 , does not support the SMART/Health information log page on a per namespace basis\n")
	 info("Byte 262 \tELPE(Error Log Page Entries):"+fmt('0x%02x',(idfy.elpe))+'\n')		
	 info("Byte 263 \tNPSS(Number of Power States Support):"+fmt('0x%02x',(idfy.npss))+'\n')			
	 info("Byte 264 \tAVSCC(Admin Vendor Specific Command Configuration):"+ fmt('0x%02x',(idfy.avscc))+'\n')			
	 info("\t\tBits 7:1  reserved")
	 info("\t\tBit 0\tset to 1 , all Admin Vendor Specific Commands use the format")
	 info('\t\t\tset to 0 , the format of all Admin Vendor Specific Commands are vendor specific\n')
	 info("Byte 265 \tAPSTA(Autonomous Power State Transition Attributes):"+fmt('0x%02x',(idfy.apsta))+'\n')			 
	 info('\t\tBits 7:1  reserved')
	 info('\t\tBit 0 \tset to 1 , support autonomous power state transitions')
	 info('\t\t\tset to 0 , does not support autonomous power state transitions\n')
	 info("Bytes 267:266 \tWCTEMP(Warning Composite Temperature Threshold):"+fmt('0x%04x',(idfy.wctemp))+'\n')			 
     info("Bytes 269:268 \tCCTEMP(Critical Composite Temperature Threshold):"+fmt('0x%04x',(idfy.cctemp))+'\n')			
	 info("Bytes 271:270 \tMTFA(Maximum Time for Firmware Activation):"+fmt('0x%04x',(idfy.mtfa))+'\n')			 
	 info("Bytes 275:272 \tHMPRE(Host Memory Buffer Preferred Size):"+fmt('0x%08x',(idfy.hmpre))+'\n')			 
	 info("Bytes 279:276 \tHMMIN(Host Memory Buffer Minimum Size):"+fmt('0x%08x',(idfy.hmmin))+'\n')			 
	// info("Bytes 295:280 \tTNVMCAP(Total NVM Capacity):")
	 var print_s=''
	 for(j=0;j<16;j++){
		print_s = print_s +fmt('%02x',(idfy.tnvmcap[16-j]))
		if((j+1)%16==0){
		//print_s= '\t' + print_s 	
	//	info('0x'+print_s)
               info("Bytes 295:280 \tTNVMCAP(Total NVM Capacity):"+'0x'+print_s)
		print_s=''
	    }
	 }
	// info("Bytes 311:296 \tUNVMCAP(Unallocated NVM Capacity):")
	 for(m=0;m<16;m++){
		print_s = print_s +fmt('%02x',(idfy.unvmcap[16-m]))
		if((m+1)%16==0){
		//print_s= '\t' + print_s 	
	//	info('0x'+print_s)
                 info("Bytes 311:296 \tUNVMCAP(Unallocated NVM Capacity):"+'0x'+print_s)
		print_s=''
	    }
	 }
	 info("Bytes 315:312 \tRPMBS(Replay Protected Memory Block Support):"+fmt('0x%08x',(idfy.rpmbs)))

	 info('\t\tBits 31:24 Access size')
	 info('\t\tBits 23:16 Total Size')
	 info('\t\tBits 15:06  reserved')
	 info('\t\tBits 05:03 Authentication Method')
	 info('\t\tBits 02:00 Number of RPMB Units')
	 //info("idfy.rsvd316:"+idfy.rsvd316)
	 info('Bytes 511:316 \tReserved')
	 
	 info("Byte 512 \tSQES(Submission Queue Entry Size):"+fmt('0x%02x',(idfy.sqes))+'\t2^n  bytes\n')
	
	 info("\t\tBits 7:4  maximum size")
	 info("\t\tBits 3:0  required size")
	 info("Byte 513 \tCQES(Completion Queue Entry Size):"+fmt('0x%02x',(idfy.cqes))+'\t2^n bytes\n')
	

	 info("\t\tBits 7:4  maximum size")
	 info("\t\tBits 3:0  required size")
	// info("idfy.rsvd514:"+idfy.rsvd514)
	 info('Bytes 515:514 \tReserved\n')
	 
	 info("Bytes 519:516 \tMN(Number of Namespaces):"+fmt('0x%08x',(idfy.nn))+'\n')
	 
	 info("Bytes 521:520 \tONCS(Optional NVM Command Support):"+fmt('0x%04x',(idfy.oncs))+'\n')
	 
	// info("write uncorrectable cmd ")
	 //info("dataset management cmd")
	 info("\t\tBits 15:6  reserved")
	 info("\t\tBit 5\tset to 1 , support reservations")
	 info("\t\t\tset to 0 , does not support reservations")
	 info("\t\tBit 4\tset to 1 , support the Save field in the Set Features command and the Select field in the Get Features\n\t\t\tcommand")
	 info("\t\t\tset to 0 , does not support the Save field in the Set Features command and the Select field in the Get\n\t\t\tFeatures command")
	 info("\t\tBit 3\tset to 1 , support the Write Zeroes command")
	 info("\t\t\tset to 0 , does not support the Write Zeroes command")
	 info("\t\tBit 2\tset to 1 , support Dataset Management command ")
	 info("\t\t\tset to 0 , does not support Dataset Management command")
	 info("\t\tBit 1\tset to 1 , support Write Uncorrectable command")
	 info("\t\t\tset to 0 , does not support Write Uncorrectable command")
	 info("\t\tBit 0\tset to 1 , support Compare command ")
	 info("\t\t\tset to 0 , does not support Compare command \n")
     
	 info("Bytes 523:522 \tFUSES(Fused Operation Support)"+fmt('0x%04x',(idfy.fuses))+'\n')

	 info("\t\tBits 15:1  reserved")
	 info("\t\tBit 0\tset to 1 , support the Compare and Write fused operation ")
	 info("\t\t\tset to 0 , does not support the Compare and Write fused operation ")
	 info("Byte 524 \tFNA(Format NVM Attributes):"+fmt('0x%02x',(idfy.fna)))
	 
	 info("\t\tBits 7:3   reserved")
	 info("\t\tBit 2\tset to 1 , cryptographic erase is supported.")
	 info('\t\t\tset to 0 , cryptographic erase is not supported.')
	 info("\t\tBit 1\tset to 1 , a cryptographic erase of a particular namespace as part of a format results in a cryptographic\n\t\t\terase of all namespaces.")
	 info('\t\t\tset to 0 , a cryptographic erase or user data erase as part of a format is performed on a per namespace\n\t\t\tbasis')
	 info("\t\tBit 0\tset to 1 , all namespaces shall be configured with the same attributes and a format of any namespace results\n\t\t\t in a format of all namespaces.")
	 info('\t\t\tset to 0 , the controller supports format on a per namespace basis\n')
	 info("Byte 525 \tVWC(Volatile Write Cache):"+fmt('0x%02x',(idfy.vwc))+'\n')
	
	 info("\t\tBits 7:1  reserved")
	 info("\t\tBit 0\tset to 1 ,a volatile write cache is present")
	 info('\t\t\tset to 0 ,a volatile write cache is not present\n')
	 info("Bytes 527:526 AWUN(Atomic Write Unit Normal):"+fmt('0x%04x',(idfy.awun))+'\n')

	 info("\tset atomic write size,FFFF indicates all")
	 info("Bytes 529:528 AWUPF(Atomic Write Unit Power Fail):"+fmt('0x%04x',(idfy.awupf))+'\n')
	
	 info("\tset atomic write size during power fail")
	 info("Byte 530 NVSCC(NVM Vendor Specific Command Configuration):"+fmt('0x%02x',(idfy.nvscc))+'\n')
	 
	 info("\t\tBits 7:1   reserved")
	 info("\t\tBits 0\tset to 1 , all NVM Vendor Specific Commands use the format ")
	 info("\t\t\tset to 0 , the format of all NVM Vendor specific Commands are vendor specific\n")
	// info("idfy.rsvd531:"+idfy.rsvd531)
	 info('Byte 531 \tReserved\n')
	 info("Bytes 533:532 \tACWU(Atomic Compare &Write Unit):"+fmt('0x%04x',(idfy.acwu))+'\n')
     info('\tThe size of the write operation guaranteed to be written atomically to the NVM across all namespaces with any\n\tsupported namespace format for a Compare and Write fused operation')
	// info("idfy.rsvd534:"+idfy.rsvd534)
	 info('Bytes 535:534 \tReserved\n')
	 info("Bytes 539:536 \tSGLS(SGL Support)"+fmt('0x%08x',(idfy.sgls))+'\n')
	
	 info("\t\tBits 31:19  reserved")
	 info("\t\tBit 18\tset to 1 , support commands that contain a data or metadata SGL of a length than the amount of data to be\n\t\t\ttransferred")
	 info("\t\t\tset to 0 , the SGL length shall be equal to the amount of data to be transferred")
	 info("\t\tBit 17\tset to 1 , use of a byte aligned contiguous physical buffer of metadata is supported")
	 info("\t\t\tset to 0 , use of a byte aligned contiguous physical buffer of metadata is not supported")
	 info("\t\tBit 16\tset to 1 , the SGL Bit Bucket descriptor is supported")
	 info("\t\t\tset to 0 , the SGL Bit Bucket descriptor is not supported")
	 info("\t\tBits 15:01  reserved")
	 info("\t\tBit 0\tset to 1 , support SGLs for the NVM Command Set including the SGL Data Block,SGL Segment,and SGL Last\n\t\t\tSegment descriptor types")
	 info("\t\t\tset to 0 , does not support SGLs for the NVM Command Set and all other bits in this field shall be cleared\n\t\t\tto 0")
	 info('Bytes 703:540 Reserved\n')
	 info('Bytes 2047:704 Reserved\n')
	// info("idfy.rsvd540:"+idfy.rsvd540)
	// info("idfy.psd:"+idfy.psd)
	for(i=0;i<32;i++){
	 switch (i){
		 case 0:
		 info("Bytes 2079:2048  Power State 0 Descriptor (PSD0)")
		 info('\t\tThe characteristics of power state 0'); break;
		 case 1:
		 info("Bytes 2111:2080  Power State 1 Descriptor (PSD1)")
		 info('\t\tThe characteristics of power state 1'); break;
		 case 2:
		 info("Bytes 2143:2112  Power State 2 Descriptor (PSD2)")
		 info('\t\tThe characteristics of power state 2'); break;
		 case 3:
		 info("Bytes 2175:2144  Power State 3 Descriptor (PSD3)")
		 info('\t\tThe characteristics of power state 3'); break;
		 case 4:
		 info("Bytes 2207:2176  Power State 4 Descriptor (PSD4)")
		 info('\t\tThe characteristics of power state 4'); break;
		 case 5:
		 info("Bytes 2239:2208  Power State 5 Descriptor (PSD5)")
		 info('\t\tThe characteristics of power state 5'); break;
		 case 6:
		 info("Bytes 2271:2240  Power State 6 Descriptor (PSD6)")
		 info('\t\tThe characteristics of power state 6'); break;
		 case 7:
		 info("Bytes 2303:2272  Power State 7 Descriptor (PSD7)")
		 info('\t\tThe characteristics of power state 7'); break;
		 case 8:
		 info("Bytes 2335:2304  Power State 8 Descriptor (PSD8)")
		 info('\t\tThe characteristics of power state 8'); break;
		 case 9:
		 info("Bytes 2367:2336  Power State 9 Descriptor (PSD9)")
		 info('\t\tThe characteristics of power state 9'); break;
		 case 10:
		 info("Bytes 2399:2368  Power State 10 Descriptor (PSD10)")
		 info('\t\tThe characteristics of power state 10'); break;
		 case 11:
		 info("Bytes 2431:2400  Power State 11 Descriptor (PSD11)")
		 info('\t\tThe characteristics of power state 11'); break;
		 case 12:
		 info("Bytes 2463:2432  Power State 12 Descriptor (PSD12)")
		 info('\t\tThe characteristics of power state 12'); break;
		 case 13:
		 info("Bytes 2495:2464  Power State 13 Descriptor (PSD13)")
		 info('\t\tThe characteristics of power state 13'); break;
		 case 14:
		 info("Bytes 2527:2496  Power State 14 Descriptor (PSD14)")
		 info('\tThe characteristics of power state 14'); break;
		 case 15:
		 info("Bytes 2559:2528  Power State 15 Descriptor (PSD15)")
		 info('\t\tThe characteristics of power state 15'); break;
		 case 16:
		 info("Bytes 2591:2560  Power State 16 Descriptor (PSD16)")
		 info('\t\tThe characteristics of power state 16'); break;
		 case 17:
		 info("Bytes 2623:2592  Power State 17 Descriptor (PSD17)")
		 info('\t\tThe characteristics of power state 17'); break;
		 case 18:
		 info("Bytes 2655:2624  Power State 18 Descriptor (PSD18)")
		 info('\t\tThe characteristics of power state 18'); break;
		 case 19:
		 info("Bytes 2687:2656  Power State 19 Descriptor (PSD19)")
		 info('\t\tThe characteristics of power state 19'); break;
		 case 20:
		 info("Bytes 2719:2688  Power State 20 Descriptor (PSD20)")
		 info('\t\tThe characteristics of power state 20'); break;
		 case 21:
		 info("Bytes 2751:2720  Power State 21 Descriptor (PSD21)")
		 info('\t\tThe characteristics of power state 21'); break;
		 case 22:
		 info("Bytes 2783:2752  Power State 22 Descriptor (PSD22)")
		 info('\t\tThe characteristics of power state 22'); break;
		 case 23:
		 info("Bytes 2815:2784  Power State 23 Descriptor (PSD23)")
		 info('\t\tThe characteristics of power state 23'); break;
		 case 24:
		 info("Bytes 2847:2816  Power State 24 Descriptor (PSD24)")
		 info('\t\tThe characteristics of power state 24'); break;
		 case 25:
		 info("Bytes 2879:2848  Power State 25 Descriptor (PSD25)")
		 info('\t\tThe characteristics of power state 25'); break;
		 case 26:
		 info("Bytes 2911:2880  Power State 26 Descriptor (PSD26)")
		 info('\t\tThe characteristics of power state 26'); break;
		 case 27:
		 info("Bytes 2943:2912  Power State 27 Descriptor (PSD27)")
		 info('\t\tThe characteristics of power state 27'); break;
		 case 28:
		 info("Bytes 2975:2944  Power State 28 Descriptor (PSD28)")
		 info('\t\tThe characteristics of power state 28'); break;
		 case 29:
		 info("Bytes 3007:2976  Power State 29 Descriptor (PSD29)")
		 info('\t\tThe characteristics of power state 29'); break;
		 case 30:
		 info("Bytes 3039:3008  Power State 30 Descriptor (PSD30)")
		 info('\t\tThe characteristics of power state 30'); break;
		 case 31:
		 info("Bytes 3071:3040  Power State 31 Descriptor (PSD31)")
		 info('\t\tThe characteristics of power state 31'); break;
		}

/* 		 info(fmt('0x%02x',(idfy.psd[i].active_work_scale))+fmt('%04x',(idfy.psd[i].active_power))
		 +fmt('%02x',(idfy.psd[i].rsvd19))+fmt('%02x',(idfy.psd[i].idle_scale))
		 +fmt('%04x',(idfy.psd[i].idle_power))+fmt('%02x',(idfy.psd[i].write_lat))
		 +fmt('%02x',(idfy.psd[i].write_tput))+fmt('%02x',(idfy.psd[i].read_lat))
		 +fmt('%02x',(idfy.psd[i].read_tput))+fmt('%08x',(idfy.psd[i].exit_lat))
		 +fmt('%08x',(idfy.psd[i].entry_lat))+fmt('%02x',(idfy.psd[i].flags))
		 +fmt('%02x',(idfy.psd[i].rsvd2))+fmt('%04x',(idfy.psd[i].max_power))) */
		 //info(+fmt('0x%02x',(idfy.psd[i].active_work_scale)))
         info('MP:'+fmt('0x%04x',(idfy.psd[i].max_power))+'   '+"FLAGS:"+fmt('0x%02x',(idfy.psd[i].flags))+'   '
		 +'ENLAT:'+fmt('0x%08x',(idfy.psd[i].entry_lat))+'   '+'EXLAT:'+fmt('0x%08x',(idfy.psd[i].exit_lat))+'   '
		 +'RRT:'+fmt('0x%02x',(idfy.psd[i].read_tput))+'   '+'RRL:'+fmt('0x%02x',(idfy.psd[i].read_lat))+'   '
		 +'RWT:'+fmt('0x%02x',(idfy.psd[i].write_tput))+'   '+"RWL:"+fmt('0x%02x',(idfy.psd[i].write_lat))+'   '
		 +'IDLP:'+fmt('0x%04x',(idfy.psd[i].idle_power))+'   '+'IPS:'+fmt('0x%02x',(idfy.psd[i].idle_scale))+'   '
		 +'ACTP:'+fmt('0x%04x',(idfy.psd[i].active_power))+'   '+'APFLAGS:'+fmt('0x%02x',(idfy.psd[i].active_work_scale))+'\n')
    
	}	
        info('Bytes 4095:3072 Vendor Specific')			
	// info("idfy.vs:"+idfy.vs)
 }
		
		 else if(para1==0){
			 var idfy=identify(para1)
			 var sector_size = iniat.sector_size
			 info("Bytes 7:0 \t NSZE(Namespace Size):"+fmt('0x%016x',(idfy.nsze))+'   '+Math.round((idfy.nsze)*sector_size/(Math.pow(10,9)))+'G') 			 
			 info('\tThe total size of the namespace in logical blocks\n')			 
			 info("Bytes 15:8 \t NCAP(Namespace Capacity):"+fmt('0x%016x',(idfy.ncap))+'   '+Math.round((idfy.ncap)*sector_size/(Math.pow(10,9)))+'G')			 
			 info('\tThe maximum number of logical blocks that may be allocated in the namespace at any point in time\n')			 
			 info("Bytes 23:16 \t NUSE(Namespace Utilization):"+fmt('0x%016x',(idfy.nuse))+'   '+Math.round((idfy.nuse)*sector_size/(Math.pow(10,9)))+'G')			 
			 info('\tThe current number of logical blocks allocated in the namespace\n')
			 info("Byte 24 \t NSFEAT(Namespace Features):"+fmt('0x%02x',(idfy.nsfeat)))			 
			 info('\t\tBit 7:3 reserved')
			 info('\t\tBit 2\tset to 1 , support the Deallocated or Unwritten Logical Block')
			 info('\t\t\tset to 0 , does not support the Deallocated or Unwritten Logical Block')
			 info('\t\tBit 1\tset to 1 , NAWUN,NAWUPF,NAWUPF,and NACWU are defined and instead of the AWUN,AWUPF,and ACWU')
			 info(' \t\t\tset to 0 , does not support NAWUN,NAWUPF,NAWUPF,and NACWU')
			 info('\t\tBit 0\tset to 1 , the namespace supports thin provisioning')
			 info(' \t\t\tset to 0 , thin provisioning is not supported\n')
			 //info("Byte 25 \t NLBAF(Number of LBA Formats):"+fmt('0x%02x',(idfy.nlbaf)))
			 info("Byte 25 \t NLBAF(Number of LBA Formats):"+fmt('0x%02x',(idfy.nlbaf)))
			 info('\tThis is a 0 based value')
			 info('\tThe number of supported LBA data size and metadata size combinations supported by the namespace\n')
			 info("Byte 26 \t FLBAS(Formatted LBA Size):"+fmt('0x%02x',(idfy.flbas)))
			 info('\tThe LBA data size & metadata size combination that the namespace has been formatted with\n')
			 info('\t\tBits 7:5 Reserved')
			 info('\t\tBit 4\tset to 1,the metadata is transferred at the end of the data LBA,creating an extended data LBA')
			 info('\t\t\tset to 0,all of the metadata for a command is transferred as a separate contiguous buffer of data')
			 info('\t\tBits 3:0 indicates one of the 16 supported LBA Formats indicated in this data structure\n')
			 info("Byte 27 \t MC(Metadata Capabilities):"+fmt('0x%02x',(idfy.mc)))
			 info("\tIndicate the capabilities for metadata\n ")
			 info('\t\tBits 7:2 Reserved')
			 info('\t\tBit 1\tset to 1,the namespace supports the metadata being transferred as part of a separate buffer that is\n\t\t\tspecified in the Metadata Pointer')
			 info('\t\t\tset to 0,the namespace not support the metadata being transferred as part of a separate buffer')
			 info('\t\tBit 0\tset to 1,the namespace supports the metadata being transferred as part of an extended data LBA')
			 info('\t\t\tset to 0,the namespace does not supports the metadata being transferred as part of an extended data LBA\n')
			 info("Byte 28 \t DPC(End-to-end Data Protection Capabilities):"+fmt('0x%02x',(idfy.dpc)))
			 info("\t\tBits 7:5 Reserved")
			 info("\t\tBit 4\tset to 1,the namespace supports protection information transferred as the last eight bytes of metadata")
			 info('\t\t\tset to 0,the namespace does not supports protection information transferred as the last eight bytes of\n\t\t\tmetadata')
			 info('\t\tBit 3\tset to 1,the namespace supports protection information transferred  as the first eight bytes of metadata')
			 info('\t\t\tset to 0,the namespace does not supports protection information transferred  as the first eight bytes of\n\t\t\tmetadata')
			 info('\t\tBit 2\tset to 1,the namespace supports Protection Information Type 3')
			 info(' \t\t\tset to 0,the namespace does not supports Protection Information Type 3')
			 info('\t\tBit 1\tset to 1,the namespace supports Protection Information Type 2')
			 info(' \t\t\tset to 0,the namespace does not supports Protection Information Type 2')
			 info('\t\tBit 0\tset to 1,the namespace supports Protection Information Type 1')
			 info(' \t\t\tset to 0,the namespace does not supports Protection Information Type 1\n')
			 info("Byte 29 \t DPS(End-to-end Data Protection Type Settings):"+fmt('0x%02x',(idfy.dps)))
			 info('\t\tBits 7:4 Reserved')
			 info('\t\tBit 3\tset to 1,the protection information,if enabled,is transferred as the first eight bytes of metadata')
			 info(' \t\t\tset to 0,the protection information,if enabled,is transferred as the last eight bytes of metadata')
			 info('\t\tBits 2:0 indicate whether Protection Information is enabled and the type of Protection Information enabled')
			 info('\t\t\t000b Protection information is not enabled')
			 info('\t\t\t001b Protection information is enabled,Type 1')
			 info('\t\t\t010b Protection information is enabled,Type 2')
			 info('\t\t\t011b Protection information is enabled,Type 3')
			 info('\t\t\t100b-111b Reserved \n')
			 info("Byte 30 \t NMIC(Namespace Multi-path I/O and Namespace Sharing Capabilities):"+fmt('0x%02x',(idfy.nmic)))
			 info('\t\tBits 7:1 Reserved')
			 info('\t\tBit 0\tset to 1,the namespace may be accessible by two or more controllers in the NVM subsystem')
			 info('\t\t\tset to 0,the namespace is a private namespace \n')
			 info("Byte 31 \t RESCAP(Reservation Capabilities):"+fmt('0x%02x',(idfy.rescap)))
			 info('\t00h indicates reservations are not supported by this namespace')
			 info('\t\tBit 7 Reserved')
			 info('\t\tBit 6\tset to 1 , the namespace supports the Exclusive Access-All Registrants reservation type')
			 info('\t\t\tset to 0 , the namespace does not supports the Exclusive Access-All Registrants reservation type')
			 info('\t\tBit 5\tset to 1 , the namespace supports the Write Exclusive-All Registrants reservation type')
			 info('\t\t\tset to 0 , the namespace does not supports the Exclusive-All Registrants reservation type')
			 info('\t\tBit 4\tset to 1 , the namespace supports the Exclusive Access-Registrants Only reservation type')
			 info(' \t\t\tset to 0 , the namespace does not supports the Exclusive Access-Registrants Only reservation type')
			 info('\t\tBit 3\tset to 1 , the namespace supports the Write Exclusive-Registrants Only reservation type')
			 info(' \t\t\tset to 0 , the namespace does not supports the Write Exclusive-Registrants Only reservation type')
			 info('\t\tBit 2\tset to 1 , the namespace supports the Exclusive Access reservation type')
			 info('\t\t\tset to 0 , the namespace does not supports the Exclusive Access reservation type')
			 info('\t\tBit 1\tset to 1 , the namespace supports the Write Exclusive reservation type')
			 info(' \t\t\tset to 0 , the namespace does not supports the Write Exclusive reservation type')
			 info('\t\tBit 0\tset to 1 , the namespace supports the Persist Through Power Loss capability')
			 info(' \t\t\tset to 0 , the namespace does not supports the Persist Through Power Loss capability\n')
			 info("Byte 32 \t FPI(Format Progress Indicator):"+fmt('0x%02x',(idfy.fpi)))
			 info('\t\tBit 7\tset to 1 , the namespace supports the Format Progress Indicator defined by bits 6:0 in this field')
			 info('\t\t\tset to 0 , the namespace does not support the Format Progress Indicator defined by bits 6:0 in this field')
			 info('\t\tBits 6:0 indicate the percentage of the namespace that remains to be formatted\n')
			// info("idfy.rsvd33:"+idfy.rsvd33)
			 info('Byte 33 \tReserved\n')
			 info("Bytes 35:34 \t NAWUN(Namespace Atomic Write Unit Normal):"+fmt('0x%04x',(idfy.nawun))+'\n')
			 info("Bytes 37:36 \t NAWUPF(Namespace Atomic Write Unit Power Fail):"+fmt('0x%04x',(idfy.nawupf))+'\n')
			 info("Bytes 39:38 \t NACWU(Namespace Atomic Compare & Write Unit):"+fmt('0x%04x',(idfy.nacwu))+'\n')
			 info("Bytes 41:40 \t NABSN(Namespace Atomic Boundary Size Normal):"+fmt('0x%04x',(idfy.nabsn))+'\n')
			 info("Bytes 43:42 \t NABO(Namespace Atomic Boundary Offset):"+fmt('0x%04x',(idfy.nabo))+'\n')
			 info("Byte 45:44 \t NABSPF(Namespace Atomic Boundary Size Power Fail):"+fmt('0x%04x',(idfy.nabspf))+'\n')
			// info("idfy.rsvd46:"+idfy.rsvd46)
			 info('Bytes 47:46\t Reserved\n')
			// info("Bytes 63:48 \t NVMCAP(NVM Capacity)\n")
			 var print_s=''
			 for(n=0;n<16;n++){

				print_s = print_s + fmt('%02x',(idfy.nvmcap[15-n]))
				if((n+1)%16==0){
				//print_s= '\t' + print_s 	
			//	info('0x'+print_s+'   '+Math.round((print_s)*512/(Math.pow(10,9)))+'G\n')
                               info("Bytes 63:48 \t NVMCAP(NVM Capacity):"+'0x'+print_s)
				print_s=''
			    }
			 } 
			 //info("idfy.rsvd64:"+idfy.rsvd64)
			 info('Bytes 103:64 \tReserved\n')
			// info("Bytes 119:104 \t NGUID(Namespace Globally Unique Identifier)\n")
			 for(aa=0;aa<16;aa++){
			     print_s = print_s +fmt('%02x',(idfy.nguid[15-aa]))
				if((aa+1)%16==0){
				//print_s= '\t' + print_s 	
			//	info('0x'+print_s+'   '+Math.round((print_s)*512/(Math.pow(10,9)))+'G\n')
                               info("Bytes 119:104 \t NGUID(Namespace Globally Unique Identifier):"+'0x'+print_s)
				print_s=''
			    }
			 }
	//		 info("Bytes 127:120 \t EUI64(IEEE Extended Unique Identifier)\n")
			 for(bb=0;bb<8;bb++){
				 print_s = print_s + fmt('%02x',(idfy.eui64[bb]))
				if((bb+1)%8==0){
				//print_s= '\t' + print_s 	
			//	info('0x'+print_s+'\n')
                               info("Bytes 127:120 \t EUI64(IEEE Extended Unique Identifier):"+'0x'+print_s)
				print_s=''
			    }
			 }
			 //info("Byte 128:  LBAF0(LBA Format 0 Support)")
			for(i=0;i<16;i++){
				switch (i){
				 case 0:
				 info("Bytes \t131:128  LBAF0(LBA Format 0 Support)")
				 info('\t\tThe LBA format 0 that is supported by the controller'); break;
				 case 1:
				 info("Bytes \t135:132  LBAF1(LBA Format 1 Support)")
				 info('\t\tThe LBA format 1 that is supported by the controller'); break;
				 case 2:
				 info("Bytes \t139:136  LBAF2(LBA Format 2 Support)")
				 info('\t\tThe LBA format 2 that is supported by the controller'); break;
				 case 3:
				 info("Bytes \t143:140  LBAF3(LBA Format 3 Support)")
				 info('\t\tThe LBA format 3 that is supported by the controller'); break;
				 case 4:
				 info("Bytes \t147:144  LBAF4(LBA Format 4 Support)")
				 info('\t\tThe LBA format 4 that is supported by the controller'); break;
				 case 5:
				 info("Bytes \t151:148  LBAF5(LBA Format 5 Support)")
				 info('\t\tThe LBA format 5 that is supported by the controller'); break;
				 case 6:
				 info("Bytes \t155:152  LBAF6(LBA Format 6 Support)")
				 info('\t\tThe LBA format 6 that is supported by the controller'); break;
				 case 7:
				 info("Bytes \t159:156  LBAF7(LBA Format 7 Support)")
				 info('\t\tThe LBA format 7 that is supported by the controller'); break;
				 case 8:
				 info("Bytes \t163:160  LBAF8(LBA Format 8 Support)")
				 info('\t\tThe LBA format 8 that is supported by the controller'); break;
				 case 9:
				 info("Bytes \t167:164  LBAF9(LBA Format 9 Support)")
				 info('\t\tThe LBA format 9 that is supported by the controller'); break;
				 case 10:
				 info("Bytes \t171:168  LBAF10(LBA Format 10 Support)")
				 info('\t\tThe LBA format 10 that is supported by the controller'); break;
				 case 11:
				 info("Bytes \t175:172  LBAF11(LBA Format 11 Support)")
				 info('\t\tThe LBA format 11 that is supported by the controller'); break;
				 case 12:
				 info("Bytes \t179:176  LBAF12(LBA Format 12 Support)")
				 info('\t\tThe LBA format 12 that is supported by the controller'); break;
				 case 13:
				 info("Bytes \t183:180  LBAF13(LBA Format 13 Support)")
				 info('\t\tThe LBA format 13 that is supported by the controller'); break;
				 case 14:
				 info("Bytes \t187:184  LBAF14(LBA Format 14 Support)")
				 info('\t\tThe LBA format 14 that is supported by the controller'); break;
				 case 15:
				 info("Bytes \t191:188  LBAF15(LBA Format 15 Support)")
				 info('\t\tThe LBA format 15 that is supported by the controller'); break;

			}
			info('Metadata Size: '+fmt('0x%04x',(idfy.lbaf[i].ms))+"   "+'LBA Data Size: '+fmt('0x%02x',(idfy.lbaf[i].ds))+"   "+'Relative Performance: '+fmt('0x%02x',(idfy.lbaf[i].rp))+'\n')
			}
			// info("idfy.rsvd192:"+idfy.rsvd192)
			 info("Bytes \t383:192  reserved")
			 //info("idfy.vs:"+idfy.vs)
			 info("Bytes \t4095:384  VS(Vendor Specific)")
		 }
		 else
			 info('Parameter is not supported ')
	
	}
	else if(cmd=='gfea'){
		
		
		if(para1==1){
			var gfea=get_features(para1)
			info('Arbitration')
			info(fmt('0x%08x',(gfea[1])))
			info('Bit 02:00 Arbitration Burst . This value is specified as 2^n')
			info('Bit 07:03 Reserved')
			info('Bit 15:08 Low Priority Weight')
			info('Bit 23:16 Medium Priority Weight')
			info('Bit 31:24 High Priority Weight')

		}
		else if(para1==2){
			var gfea=get_features(para1)
			info('Power Management')
			info(fmt('0x%08x',(gfea[1])))
			info('Bit 04:00 Power State')
			     info('This power state shall be one supported by the controller as indicated in the NPSS field in the Identify Controller data structure')
				 info('This power state is not supported , the controller should return an error of Invalid Field in Command')
			info('Bit 07:05 Workload Hint')
				 info('\t000b No Workload')
				 info('\t001b Workload #1')
				 info('\t010b Workload #2')
			info('Bit 31:08 Reserved')
		}
		else if(para1==3){
			var gfea=get_features(para1)
			info('LBA Range Type ')
			//info('0x'+gfea.toString(16))
			if((gfea[0])!=0)
				info('This feature is not supported')
		}
		else if(para1==4){
			var gfea=get_features(para1)
			info('Temperature Threshold')
			info(fmt('0x%08x',(gfea[1])))
			info('Bit 15:00 Temperature Threshold. the threshold value to apply or return for the temperature sensor and threshold type specified')
			info('Bit 19:16 Threshold Temperature Select')
                info('\t0000b Composite Temperature')		
                info('\t0001b Temperature Sensor 1')	
				info('\t0010b Temperature Sensor 2')
				info('\t0011b Temperature Sensor 3')	
				info('\t0100b Temperature Sensor 4')	
				info('\t0101b Temperature Sensor 5')	
				info('\t0110b Temperature Sensor 6')	
				info('\t0111b Temperature Sensor 7')	
				info('\t1000b Temperature Sensor 8')
                info('\t1111b Reserved in a Get Features command')
            info('Bit 21:20 Threshold Type Select')	
                info('\t00b  Over Temperature Threshold')	
                info('\t01b  Under Temperature Threshold')
			info('Bit 31:22 Reserved')
		}
		else if(para1==5){
			var gfea=get_features(para1)
			info('Error Recovery')
			info(fmt('0x%08x',(gfea[1])))
			info('Bit 15:00 Time Limited Error Recovery. A value of 0h indicates that there is no timeout')
            info('Bit 16 Deallocated or Unwritten Logical Block Error Enable')
                info('\tset to 1,the Deallocated or Unwritten Logical Block error is enabled for the namespace specified in CDW1.NSID')	
                info('\tset to 0,the Deallocated or Unwritten Logical Block error is disabled for the namespace specified in CDW1.NSID')
            info('Bit 31:17 Reserved')				
		}
		else if(para1==6){
			var gfea=get_features(para1)
			info('Volatile Write Cache')
			info(fmt('0x%08x',(gfea[1])))
			info('Bit 00: Volatile Write Cache Enable')
			    info('\tset to 1,the volatile write cache is enabled')
				info('\tset to 0,the volatile write cache is disabled')	
			info('Bit 31:01 Reserved')
		}
		else if(para1==7){
			var gfea=get_features(para1)
			info('Number of Queues')
			info(fmt('0x%08x',(gfea[1])))
			info('Submission Queues '+((gfea[1]&0xffff)+1))
			info('Completion Queues '+(((gfea[1]>>16)&0xffff)+1))
			info('Bit 15:00 Number of I/O Submission Queues Requested')
			info('Bit 31:16 Number of I/O Completion Queues Requested')
			
		}
		else if(para1==8){
			var gfea=get_features(para1)
			info('Interrupt Coalescing')
			info(fmt('0x%08x',(gfea[1])))
			info('Bit 07:00 Aggregation Threshold. This is a 0 based value ')
			info('Bit 15:08 Aggregation Time. A value of 0h corresponds to no delay')		    
			info('Bit 31:16 Reserved')
		}
		else if(para1==9){
			var gfea=get_features(para1)
			info('Interrupt Vector Configuration')
			info(fmt('0x%08x',(gfea[1])))
            info('Bit 15:00 Interrupt Vector. the interrupt vector for the configuration settings are applied')
			info('Bit 16 Coalescing Disable. ')
			    info('\tset to 1,any interrupt coalescing settings shall not be applied for this interrupt vector.')
		        info('\tset to 0, interrupt coalescing settings apply for this interrupt vector.')
			info('Bit 31:17 Reserved')
		}
		else if(para1==10){
			var gfea=get_features(para1)
			info('Write Atomicity Normal')
			info(fmt('0x%08x',(gfea[1])))
			info('Bit 00 Disable Normal.')
			    info('\tset to 1,the host specifies the AWUN and NAWUN are not required and the controller shall only honor AWUPF and NAWUPF.')
			    info('\tset to 0, AWUN , NAWUN ,AWUPF,and NAWUPF shall be honored by the controller.')
			info('Bit 31:01 Reserved')
			
		}
		else if(para1==11){
			var gfea=get_features(para1)
			info('Asynchronous Event Configuration')
			info(fmt('0x%08x',(gfea[1])))
            info('Bit 07:00 SMART/Health Critical Warnings')
			     info('\tset to 1 , an asynchronous event notification is sent when the corresponding critical warning bit is set to 1')
				 info('\tset to 0 , an asynchronous event notification is not sent when the corresponding critical warning bit is set to 1')
			info('Bit 08 Namespace Attribute Notices')
			     info('\tset to 1 , the Namespace Attribute Changed event is sent to the host when this condition occurs')
				 info('\tset to 0 , the controller shall not send the Namespace Attribute Changed event to the host')
			info('Bit 09 Firmware Activation Notices.')	 
			     info('\tset to 1 , the Firmware Activation Starting event is sent to the host when this condition occurs')
				 info('\tset to 0 , the controller shall not send the Firmware Activation Starting event to the host')
			info('Bit 31:10 Reserved')
		}
		else
			info('Parameter is not supported ')
		
		

	}
	else if(cmd=='glog'){
		var print_s = ''
		//var glog=get_log_page(para1,para2)
		if(para2==1){
			//para1=64
			var glog=get_log_page(para1,para2)
			info('Error Information ')			
			for(i = 0; i < para1; i++){
			switch(i){
				case 0:  info("Byte 07:00   Error Count"); break;
				case 8:  info("Byte 09:08   Submission Queue ID");break;
				case 10: info("Byte 11:10   Command ID"); break;
				case 12: info("Byte 13:12   Status Field")		;  break;
				case 14: info("Byte 15:14   Parameter Error Location");            break;
				case 16: info("Byte 23:16   LBA");          break;
				case 24: info("Byte 27:24   Namespace");                           break;
				case 28: info("Byte 28   Vendor Specific Information Available");break;
				case 29: info("Byte 31:29   Reserved");                             break;
				case 32: info("Byte 39:32   Command Specific Information") ;         break;
				case 40: info("Byte 63:40   Reserved");                             break;
			}
			
			if(i<8){
				//print_s = print_s + "  "+fmt('0x%02x',(glog[7-i]))
				print_s = print_s +fmt('%02x',(glog[7-i]))
				if((i+1)%8==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				info('This is a 64-bit incrementing error count,indicating a unique identifier for this error\n')
				print_s=''
			    }
				
			}
			
			else if(i>=8&&i<40){
				if(i<10){
					print_s = print_s +fmt('%02x',(glog[17-i]))
					if((i+1)%2==0){
					//print_s= '\t' + print_s 	
					info('0x'+print_s)
					info('Indicates the Submission Queue Identifier of the command that the error information is associated with\n')
					print_s=''
					}
				//	info(fmt('0x%02x',(glog[17-i])))
				
				}
				else if(i>=10&&i<12){
					print_s = print_s +fmt('%02x',(glog[21-i]))
					if((i+1)%2==0){
					//print_s= '\t' + print_s 	
					info('0x'+print_s)
					info('Indicates the Command Identifier of the command that the error is associated with\n')
					print_s=''
					}
					//info(fmt('0x%02x',(glog[21-i])))
					
				}
				else if(i>=12&&i<14){
					print_s = print_s +fmt('%02x',(glog[25-i]))
					if((i+1)%2==0){
					//print_s= '\t' + print_s 	
					info('0x'+print_s)
					info('Indicates the Status Field for the command that completed\n')
					print_s=''
					}
				//	info(fmt('0x%02x',(glog[25-i])))
				
				}
				else if(i>=14&&i<16){
					print_s = print_s +fmt('%02x',(glog[29-i]))
					if((i+1)%2==0){
					//print_s= '\t' + print_s 	
					info('0x'+print_s)
					info('Bit 7:0 Byte in command that contained the error.Valid values are 0 to 63')
					info('Bit 10:8 Bit in command that contained the error.Valid values are 0 to 7')
				    info('Bit 15:11 Reserved\n')
					print_s=''
					}
				//	info(fmt('0x%02x',(glog[25-i])))
				      
				}
				else if(i>=16&&i<24){
					print_s = print_s +fmt('%02x',(glog[39-i]))
					if((i+1)%8==0){
					//print_s= '\t' + print_s 	
					info('0x'+print_s)
					info('Indicates the first LBA that experienced the error condition,if applicable\n')
					print_s=''
					}
				//	info(fmt('0x%02x',(glog[25-i])))
				
				}
				else if(i>=24&&i<28){
					print_s = print_s +fmt('%02x',(glog[51-i]))
					if((i+1)%4==0){
					//print_s= '\t' + print_s 	
					info('0x'+print_s)
					info('Indicates the namespace that the error is associated with,if applicable\n')
					print_s=''
					}
				//	info(fmt('0x%02x',(glog[25-i])))
				 
				}
				else if(i==28){
					info(fmt('0x%02x',(glog[28])))
					info('Additional vendor specific error information available\n')
				}
				else if(i>=32&&i<40){
					print_s = print_s +fmt('%02x',(glog[71-i]))
					if((i+1)%8==0){
					//print_s= '\t' + print_s 	
					info('0x'+print_s)
					info('Contains command specific information\n')
					print_s=''
					}
                }
			/* else{

			   }*/	
				
			}
			}
		}
		else if(para2==2){
			var glog=get_log_page(para1,para2)
			info('SMART / Health Information')	
				//para1=512	
			for(i = 0; i < para1; i++){
			switch(i){
				case 0:   info("Byte 0:    Critical Warning");                       break;
				case 1:   info("Bytes 2:1    Composite Temperature");                  break;
				case 3:   info("Byte 3:    Available Spare");                        break;
				case 4:   info("Byte 4:    Available Spare Threshold");              break;
				case 5:   info("Byte 5:     percentage Used");                       break;
				case 6:   info("Bytes 31:6     Reserved");                              break;
				case 32:  info("Bytes 47:32    Data Units Read");                        break;
				case 48:  info("Bytes 63:48    Data Units written");                     break;
				case 64:  info("Bytes 79:64    Host Read Commands");                     break;
				case 80:  info("Bytes 95:80    Host Write Commands");                    break;
				case 96:  info("Bytes 111:96    Controller Busy Time");                   break;
				case 112: info("Bytes 127:112   Power Cycles");                           break;
				case 128: info("Bytes 143:128   Power On Hours");                         break;
				case 144: info("Bytes 159:144   Unsafe Shutdowns");                       break;
				case 160: info("Bytes 175:160   Media and Data Integrity Errors");        break;
				case 176: info("Bytes 191:176   Number of Error Information Log Entries");break;
				case 192: info("Bytes 195:192   Warning Composite Temperature Time");     break;
				case 196: info("Bytes 199:196   Critical Composite Temperature Time");    break;
				case 200: info("Bytes 201:200   Temperature Sensor 1");                   break;
				case 202: info("Bytes 203:202   Temperature Sensor 2");                   break;
				case 204: info("Bytes 205:204   Temperature Sensor 3");                   break;
				case 206: info("Bytes 207:206   Temperature Sensor 4");                   break;
				case 208: info("Bytes 209:208   Temperature Sensor 5");                   break;
				case 210: info("Bytes 211:210   Temperature Sensor 6");                   break;
				case 212: info("Bytes 213:212   Temperature Sensor 7");                   break;
				case 214: info("Bytes 215:214   Temperature Sensor 8");                   break;
				case 216: info("Bytes 511:216   Reserved");                               break;
			}
			if(i<6){
                if(i==0){
					info(fmt('0x%02x',(glog[i])))						 
						  info('\tBits 07:05 Reserved')
						  info('\tBit 04: set to 1,the volatile memory backup device has failed')
						  info('\tBit 03: set to 1,the media has been placed in read only mode')
						  info('\tBit 02: set to 1,the NVM subsystem reliability has been degraded due to significant media related errors or any internal error')
						  info('\tBit 01: set to 1, a temperature is above an over temperature threshold or below an under temperature threshold')
						  info('\tBit 00: set to 1,the available spare space has fallen below the threshold\n')
				}
				
				else if(i==2||i==1){
					print_s = print_s + fmt('%02x',(glog[3-i]))
					if((i+1)%3==0){
					//print_s= '        ' + print_s 	
					info('0x'+print_s)
					info('A value corresponding to a temperature\n')
					print_s=''
				   }	
				}
				else if (i==3){
					info(fmt('0x%02x',(glog[i])))	
					info('Contains a normalized percentage of the remaining spare capacity available\n')
				}
				else{
					info(fmt('0x%02x',(glog[i])))
				}

				
			}

			else if(i>=32&&i<=47){
			   // print_s = print_s + "  "+fmt('0x%02x',(glog[79-i]))
			   print_s = print_s + fmt('%02x',(glog[79-i]))
				if((i+1)%16==0){
				//print_s= '        ' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
			}
			else if(i>=48&&i<64){
			   // print_s = print_s + "  "+fmt('0x%02x',(glog[111-i]))
			   print_s = print_s + fmt('%02x',(glog[111-i]))
				if((i+1)%16==0){
			//	print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
			}
			else if(i>=64&&i<80){
			    //print_s = print_s + "  "+fmt('0x%02x',(glog[143-i]))
				print_s = print_s +fmt('%02x',(glog[143-i]))
				if((i+1)%16==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				info('Contains the number of read commands completed by the controller\n')
				print_s=''
			   }	
			}
			else if(i>=80&&i<96){
			   // print_s = print_s + "  "+fmt('0x%02x',(glog[175-i]))
			   print_s = print_s + fmt('%02x',(glog[175-i]))
				if((i+1)%16==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				info('Contains the number of write commands completed by the controller\n')
				print_s=''
			   }	
			}
			else if(i>=96&&i<112){
			    //print_s = print_s + "  "+fmt('0x%02x',(glog[207-i]))
				print_s = print_s + fmt('%02x',(glog[207-i]))
				if((i+1)%16==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
			}
			else if(i>=112&&i<128){
			    //print_s = print_s + "  "+fmt('0x%02x',(glog[239-i]))
				print_s = print_s + fmt('%02x',(glog[239-i]))
				if((i+1)%16==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				info('Contains the number of power cycles\n')
				print_s=''
			   }	
			}
			else if(i>=128&&i<144){
			    //print_s = print_s + "  "+fmt('0x%02x',(glog[271-i]))
				print_s = print_s +fmt('%02x',(glog[271-i]))
				if((i+1)%16==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				info('Contains the number of power-on hours\n')
				print_s=''
			   }	
			}
			else if(i>=144&&i<160){
			    //print_s = print_s + "  "+fmt('0x%02x',(glog[303-i]))
				print_s = print_s + fmt('%02x',(glog[303-i]))
				if((i+1)%16==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				info('Contains the number of unsafe shutdowns\n')
				print_s=''
			   }	
			}
			else if(i>=160&&i<176){
			   // print_s = print_s + "  "+fmt('0x%02x',(glog[335-i]))
			   print_s = print_s + fmt('%02x',(glog[335-i]))
				if((i+1)%16==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				info('Contains the number of occurrences where the controller detected an unrecovered data integrity error\n')
				print_s=''
			   }	
			}
			else if(i>=176&&i<192){
			    //print_s = print_s + "  "+fmt('0x%02x',(glog[367-i]))
				print_s = print_s + fmt('%02x',(glog[367-i]))
				if((i+1)%16==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				info('Contains the number of Error Information log entries over the life of the controller\n')
				print_s=''
			   }	
			}
			else if(i>=192&&i<216){
				if(i>=192&&i<196){
					print_s = print_s + fmt('%02x',(glog[387-i]))
					if((i+1)%4==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
					//info(fmt('0x%02x',(glog[387-i])))
				}
				else if(i>=196&&i<200){
					print_s = print_s + fmt('%02x',(glog[395-i]))
					if((i+1)%4==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
					//info(fmt('0x%02x',(glog[395-i])))
				}
				else if(i>=200&&i<202){
					print_s = print_s + fmt('%02x',(glog[401-i]))
					if((i+1)%2==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
					//info(fmt('0x%02x',(glog[401-i])))
				}
				else if(i>=202&&i<204){
					print_s = print_s + fmt('%02x',(glog[405-i]))
					if((i+1)%2==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
					//info(fmt('0x%02x',(glog[405-i])))
				}
				else if(i>=204&&i<206){
					print_s = print_s + fmt('%02x',(glog[409-i]))
					if((i+1)%2==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
					//info(fmt('0x%02x',(glog[409-i])))
				}
				else if(i>=206&&i<208){
					print_s = print_s + fmt('%02x',(glog[413-i]))
					if((i+1)%2==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
					//info(fmt('0x%02x',(glog[413-i])))
				}
				else if(i>=208&&i<210){
					print_s = print_s + fmt('%02x',(glog[417-i]))
					if((i+1)%2==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
					//info(fmt('0x%02x',(glog[417-i])))
				}
				else if(i>=210&&i<212){
					print_s = print_s + fmt('%02x',(glog[421-i]))
					if((i+1)%2==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
					//info(fmt('0x%02x',(glog[421-i])))
				}
				else if(i>=212&&i<214){
					print_s = print_s + fmt('%02x',(glog[425-i]))
					if((i+1)%2==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
					//info(fmt('0x%02x',(glog[425-i])))
				}
				}
				else if(i>=214&&i<216){
					print_s = print_s + fmt('%02x',(glog[429-i]))
					if((i+1)%2==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''

				}
				

			}
			
		}
			
		}
		}
		else if(para2==3){
			var glog=get_log_page(para1,para2)
			info('Firmware Slot Information ')
			//para1=512
			for(i=0;i<para1;i++){
				switch(i){
				case 0:   info("Byte 00    Active Firmware Info");               break;
				case 1:   info("Bytes 07:01    Reserved");                           break;
				case 8:   info("Bytes 15:08    Firmware Revision for Slot 1");       break;
				case 16:  info("Bytes 23:16    Firmware Revision for Slot 2 ");      break;
				case 24:  info("Bytes 31:24    Firmware Revision for Slot 3");       break;
				case 32:  info("Bytes 39:32    Firmware Revision for Slot 4");       break;
				case 40:  info("Bytes 47:40    Firmware Revision for Slot 5");       break;
				case 48:  info("Bytes 55:48    Firmware Revision for Slot 6");       break;
				case 56:  info("Bytes 63:56    Firmware Revision for Slot 7");       break;
				case 64:  info("Bytes 511:64    Reserved");       break;	
			}
			//info('0x'+glog[i].toString(16))
			if(i==0){
				info(fmt('0x%02x',(glog[i])))
			}
/* 			else if (i>=1&&i<8){
				print_s = print_s + "  "+fmt('0x%02x',(glog[8-i]))
				if((i+1)%8==0){
				print_s= '\t' + print_s 	
				info(print_s)
				print_s=''
			   }	
			} */
			else if(i>=8&&i<16){
				//print_s = print_s + "  "+fmt('0x%02x',(glog[23-i]))
				print_s = print_s + fmt('%02x',(glog[23-i]))
				if((i+1)%8==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
			}
			else if(i>=16&&i<24){
				// print_s = print_s + "  "+fmt('0x%02x',(glog[39-i]))
				print_s = print_s + fmt('%02x',(glog[39-i]))
				if((i+1)%8==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
			}
			else if(i>=24&&i<32){
				// print_s = print_s + "  "+fmt('0x%02x',(glog[55-i]))
				print_s = print_s +fmt('%02x',(glog[55-i]))
				if((i+1)%8==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
			}
			else if(i>=32&&i<40){
				// print_s = print_s + "  "+fmt('0x%02x',(glog[71-i]))
				print_s = print_s + fmt('%02x',(glog[71-i]))
				if((i+1)%8==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
			}
			else if(i>=40&&i<48){
				// print_s = print_s + "  "+fmt('0x%02x',(glog[87-i]))
				print_s = print_s + fmt('%02x',(glog[87-i]))
				if((i+1)%8==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
			}
			else if(i>=48&&i<56){
				//print_s = print_s + "  "+fmt('0x%02x',(glog[103-i]))
				print_s = print_s + fmt('%02x',(glog[103-i]))
				if((i+1)%8==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
			}
			else if(i>=56&&i<64){
				// print_s = print_s + "  "+fmt('0x%02x',(glog[119-i]))
				print_s = print_s +fmt('%02x',(glog[119-i]))
				if((i+1)%8==0){
				//print_s= '\t' + print_s 	
				info('0x'+print_s)
				print_s=''
			   }	
			}
			/* else
				info('Reserved') */
		}
		}
		else{
			info('Parameter is not supported ')
		}
		}
		
	else if(cmd=='write'){
		//w_buff.fill(Number(para3),Number(para1)*512,(Number(para2)+Number(para1))*512)
		if (para3 == undefined)
		{
			pat.pat_init()
			pat.fill(Number(para1),Number(para2))//pat.fill(data,sector_num)

		}
		else
		{

			w_buff.fill(Number(para3),0,Number(para2)*512)
		}
		
		if(para2>0&&para2<257){
	    write(Number(para1),Number(para2),0)
		
    	var print_str =""
		for (i = 0; i < ((para2)*iniat.sector_size); i++){

			print_str = print_str + "  "+fmt('0x%02x',(w_buff[i]))//'0x'+(r_buff[i]).toString(16)
			if((i+1)%16==0){
			print_str= "Line:" + Math.floor(i/16)+'\t' + print_str 	
			info(print_str)
			print_str=''
			}
			
		}	
		}
		else
			info('Parameter is not supported ')
	}
	else if(cmd=='read'){
		
		
	    if(para2>0&&para2<257){
			read(Number(para1),Number(para2),Number(0))
		    var print_s=''
			for (i = 0; i < para2*iniat.sector_size; i++){
			print_s = print_s + "  "+ fmt('0x%02x',(r_buff[i]))//'0x'+r_buff[i].toString(16)
				if((i+1)%16==0){
					print_s= "Line:" + Math.floor(i/16)+'\t' + print_s 	
					info(print_s)
					print_s=''
				}
		}
		
		}
        else
			info('Parameter is not supported ')
	}
    else if(cmd=='gpci'){
		var gpci=get_pci_reg()

		var print_st=''
		var prin=''
		info('System Bus(PCI Express) Registers')
			for (i=0;i<16;i++){
					//info(fmt('0x%02x',i)+" ")
					prin = prin + "  "+ fmt('0x%02x',i)
					
				}
				info("OFFSET"+"  "+prin)
				info('--------------------------------------------------------------------------------------------------------')
			for(i = 0; i < 256; i++){
			
				print_st = print_st + "  "+ fmt('0x%02x',(gpci[i]))
				if((i+1)%16==0){
					print_st= fmt('0x%04x',i-15)+'\t' + print_st 
                    info(print_st)	
                    print_st=''					
				}
		}	
	}
	else if(cmd=='cfgpmcap'){
		var cpmcap=cfgpmcap()
		//info('offset'+'  '+'0x'+cpmcap.toString(16))
		info('offset'+'  '+fmt('0x%02x',cpmcap))
		if(cpmcap==0){
			info('Not support')
		}
		else{
		var cfgrea=cfgread16(cpmcap)
		var cfgre=cfgread16(cpmcap+2)
		var cfgr=cfgread16(cpmcap+4)
	//	info('0x'+(cfgrea).toString(16))
		info(fmt('0x%04x',cfgrea))
		info('PID-PCI Power Management Capability ID')
		info('\tBits 07:00 Cap ID')
		info('\tBits 15:08 Next Capability')
		//info('offset'+'  '+'0x'+(cpmcap+2).toString(16))
		info('offset'+'  '+fmt('0x%02x',(cpmcap+2)))
		info(fmt('0x%04x',(cfgre)))
		info('PC-PCI Power Management Capabilities')
		info('\tBits 02:00 Version')
		info('\tBit 03 PME Clock')
		info('\tBit 04 Reserved')
		info('\tBit 05 Device Specific Initialization')
		info('\tBits 08:06 Aux_Current')
		info('\tBit 09 D1_Support')
		info('\tBit 10 D2_Support')
		info('\tBits 15:11 PME_Support')
		//info('offset'+'  '+'0x'+(cpmcap+4).toString(16))
		info('offset'+'  '+fmt('0x%02x',(cpmcap+4)))
		info(fmt('0x%04x',(cfgr)))
		info('PMCS-PCI Power Management Control and Status')
		info('\tBits 01:00 Power State')
		info('\tBit 02 Reserved')
		info('\tBit 03 No Soft Reset')
		info('\tBits 07:04 Reserved')
		info('\tBit 08 PME Enable')
		info('\tBits 12:09 Data Select')
		info('\tBits 14:13 Data Scale')
		info('\tBit 15 PME Status')
		}

	}
	else if(cmd=='cfgmsicap'){
		var cmscap=cfgmsicap()
		//info('offset'+'  '+'0x'+cmscap.toString(16))
		info('offset'+'  '+fmt('0x%02x',cmscap))
		if(cmscap==0){
			info('Msi not support')
		}
		else{
		var cfgrea=cfgread16(cmscap)
		var cfgre=cfgread16(cmscap+2)
		var cfgr=cfgread32(cmscap+4)
		var cfg=cfgread32(cmscap+8)
		var cf=cfgread16(cmscap+12)
		var c=cfgread32(cmscap+16)
		var cc=cfgread32(cmscap+20)
		info('0x'+(cfgrea).toString(16))
		
		info('MID-Message Signaled Interrupt Identifiers')
		info('\tBits 07:00 Capability')
		info('\tBits 15:08 Next Pointer')
		info('0x'+(cfgre).toString(16))
		info('MC-Message Signaled Interrupt Message Control')
		info('\tBit 00 MSI Enable')
		info('\tBits 03:01 Multiple Message Capable')
		info('\tBits 06:04 Multiple Message Enable')
		info('\tBit 07 64 Bit Address Capable')
		info('\tBit 08 Per-Vector Masking Capable')
		info('\tBits 15:09 Reserved')
		info('0x'+(cfgr).toString(16))
		info('MA-Message Signaled Interrupt Message Address')
		info('\tBits 01:00 Reserved')
		info('\tBits 31:02 Address')
		info('0x'+(cfg).toString(16))
		info('Message Signaled Interrupt Upper Address')
		info('\tBits 31:00 Upper Address')
		info('0x'+(cf).toString(16))
		info('MD-Message Signaled Interrupt Message Data')
		info('\tBits 15:00 Data')
		info('0x'+(c).toString(16))
		info('MMASK-Message Signaled Interrupt Mask Bits')
		info('\tBits 31:00 Mask Bits')
		info('0x'+(cc).toString(16))
		info('MPEND-Message Signaled Interrupt Pending Bits')
		info('    Bits 31:00 Pending Bits')
		}

	}
	else if(cmd=='cfgpxcap'){
		var pxcap=cfgpxcap()
		//info('offset'+'  '+'0x'+pxcap.toString(16))
		info('offset'+'  '+fmt('0x%02x',(pxcap)))
		if(pxcap==0){
			info('Not support')
		}
		else{
		var cfgrea=cfgread16(pxcap)
		var cfgre=cfgread16(pxcap+2)
		var cfgr=cfgread32(pxcap+4)
		var cfg=cfgread16(pxcap+8)
		var cf=cfgread16(pxcap+10)
		var cc=cfgread32(pxcap+12)
		var ccf=cfgread16(pxcap+16)
		var ccfg=cfgread16(pxcap+18)
		var ccfgr=cfgread32(pxcap+36)
		var ccfgre=cfgread32(pxcap+40)
		info(fmt('0x%04x',(cfgrea)))
		//info('0x'+(cfgrea).toString(16))
		info('PXID-PCI Express Capability ID')
		info('\tBits 7:0 Capability')
		info('\tBits 15:8 Next Pointer')
		//info('offset'+'  '+'0x'+(pxcap+2).toString(16))
		info('offset'+'  '+fmt('0x%02x',(pxcap+2)))
		info(fmt('0x%04x',(cfgre)))
		//info('0x'+(cfgre).toString(16))
		info('PXCAP-PCI Express Capabilities')
		info('\tBits 3:0 Capability Version')
		info('\tBits 7:4 Device/Port Type')
		info('\tBit 8 Slot Implemented')
		info('\tBits 13:9 Interrupt Message Number')
		info('\tBits 15:14 Reserved')
		//info('offset'+'  '+'0x'+(pxcap+4).toString(16))
		info('offset'+'  '+fmt('0x%02x',(pxcap+4)))
		info(fmt('0x%08x',(cfgr)))
		//info('0x'+(cfgr).toString(16))
		info('PXDCAP-PCI Express Device Capabilities')
		info('\tBits 02:00 Max_Payload_Size Supported')
		info('\tBits 04:03 Phantom Functions Supported')
		info('\tBit 05 Extended Tag Field Supported')
		info('\tBits 08:06 Endpoint L0s Acceptable Latency')
		info('\tBits 11:9 Endpoint L1 Acceptable Latency')
		info('\tBits 14:12 Reserved')
		info('\tBit 15 Role-based Error Reporting')
		info('\tBits 17:16 Reserved')
		info('\tBits 25:18 Captured Slot Power Limit Value')
		info('\tBits 27:26 Captured Slot Power Limit Scale')
		info('\tBit 28 Function Level Reset Capability')
		info('\tBits 31:29 Reserved')
		//info('offset'+'  '+'0x'+(pxcap+8).toString(16))
		info('offset'+'  '+fmt('0x%02x',(pxcap+8)))
		info(fmt('0x%04x',(cfg)))
		//info('0x'+(cfg).toString(16))
		info('PXDC-PCI Express Device Control')
		info('\tBit 00 Correctable Error Reporting Enable')
		info('\tBit 01 Non-Fatal Error Reporting Enable')
		info('\tBit 02 Fatal Error Reporting Enable')
		info('\tBit 03 Unsupported Request Reporting Enable')
		info('\tBit 04 Enable Relaxed Ordering')
		info('\tBits 07:05 Max_Payload_Size')
		info('\tBit 08 Extended Tag Enable')
		info('\tBit 09 Phantom Functions Enable')
		info('\tBit 10 AUX Power PM Enable')
		info('\tBit 11 Enable No Snoop')
		info('\tBits 14:12 Max_Read_Request_Size')
		info('\tBit 15 Initiate Function Level Reset')
		//info('offset'+'  '+'0x'+(pxcap+10).toString(16))
		info('offset'+'  '+fmt('0x%02x',(pxcap+10)))
		info(fmt('0x%04x',(cf)))
		//info('0x'+(cf).toString(16))
		info('PXDS-PCI Express Device Status')
		info('\tBit 00 Correctable Error Detected')
		info('\tBit 01 Non-Fatal Error Detected')
		info('\tBit 02 Fatal Error Detected')
		info('\tBit 03 Unsupported Request Detected')
		info('\tBit 04 AUX Power Detected')
		info('\tBit 05 Transactions Pending')
		info('\tBits 15:06 Reserved')
		//info('offset'+'  '+'0x'+(pxcap+12).toString(16))
		info('offset'+'  '+fmt('0x%02x',(pxcap+12)))
		info(fmt('0x%08x',(cc)))
		//info('0x'+(cc).toString(16))
		info('PXLCAP-PCI Express Link Capabilities')
		info('\tBits 03:00 Supported Link Speeds')
		info('\tBits 09:04 Maximum Link Width')
		info('\tBits 11:10 Active State Power Management Support')
		info('\tBits 14:12 L0s Exit Latency')
		info('\tBits 17:15 L1 Exit Latency')
		info('\tBit 18 Clock Power Management')
		info('\tBit 19 Surprise Down Error Reporting Capable')
		info('\tBit 20 Data Link Layer Link Active Reporting Capable')
		info('\tBit 21 Link Bandwidth Notification Capability')
		info('\tBit 22 ASPM Optionality Compliance')
		info('\tBit 23 Reserved')
		info('\tBits 31:24 Port Number')
		//info('offset'+'  '+'0x'+(pxcap+16).toString(16))
		info('offset'+'  '+fmt('0x%02x',(pxcap+16)))
		info(fmt('0x%04x',(ccf)))
		//info('0x'+(ccf).toString(16))
		info('PXLC-PCI Express Link Control')
		info('\tBits 01:00 Active State Power Management Control')
		info('\tBit 02 Reserved')
		info('\tBit 03 Read Completion Boundary')
		info('\tBit 05:04 Reserved')
		info('\tBit 06 Common Clock Configuration')
		info('\tBit 07 Extended Synch')
		info('\tBit 08 Enable Clock Power Management')
		info('\tBit 09 Hardware Autonomous Width Disable')
		info('\tBits 15:10 Reserved')
		//info('offset'+'  '+'0x'+(pxcap+18).toString(16))
		info('offset'+'  '+fmt('0x%02x',(pxcap+18)))
		info(fmt('0x%04x',(ccfg)))
		//info('0x'+(ccfg).toString(16))
		info('PXLS-PCI Express Link Status')
		info('\tBits 03:00 Current Link Speed')
		info('\tBits 09:04 Negotiated Link Width')
		info('\tBits 11:10 Reserved')
		info('\tBit 12 Slot Clock Configuration')
		info('\tBits 15:13 Reserved')
		//info('offset'+'  '+'0x'+(pxcap+36).toString(16))
		info('offset'+'  '+fmt('0x%02x',(pxcap+36)))
		info(fmt('0x%08x',(ccfgr)))
		//info('0x'+(ccfgr).toString(16))
		info('PXDCAP2-PCI Express Device Capabilities')
		info('\tBits 03:00 Completion Timeout Ranges Supported')
		info('\tBit 04 Completion Timeout Disable Supported')
		info('\tBit 05 ARI Forwarding Supported')
		info('\tBit 06 AtomicOp Routing Supported')
		info('\tBit 07 32-bit AtomicOp Completer Supported')
		info('\tBit 08 64-bit AtomicOp Completer Supported')
		info('\tBit 09 128-bit CAS Completer Supported')
		info('\tBit 10 No RO-enabled PR-PR Passing')
		info('\tBit 11 Latency Tolerance Reporting Supported')
		info('\tBits 13:12 TPH Completer Supported')
		info('\tBits 17:14 Reserved')
		info('\tBits 19:18 OBFF Supported')
		info('\tBit 20 Extended Fmt Field Supported')
		info('\tBit 21 End-End TLP Prefix Supported')
		info('\tBits 23:22 Max End-End TLP Prefixes')
		info('\tBits 31:24 Reserved')
		//info('offset'+'  '+'0x'+(pxcap+40).toString(16))
		info('offset'+'  '+fmt('0x%02x',(pxcap+40)))
		info(fmt('0x%08x',(ccfgre)))
		//info('0x'+(ccfgre).toString(16))
		info('PXDC2-PCI Express Device Control 2')
		info('\tBits 03:00 Completion Timeout Value')
		info('\tBit 04 Completion Timeout Disable')
		info('\tBits 09:05 Reserved')
		info('\tBit 10 Latency Tolerance Reporting Mechanism Enable')
		info('\tBits 12:11 Reserved')
		info('\tBits 14:13 OBFF Enable')
		info('\tBits 31:15 Reserved')
		}
		
	}
	else if(cmd=='cfgmsixcap'){
        
		var cmsicap=cfgmsixcap()
		//info('offset'+'  '+'0x'+cmsicap.toString(16))
		info('offset'+'  '+fmt('0x%02x',(cmsicap)))
		if(cmsicap==0){
			info('Not support')
		}
		else{
		var cfgrea=cfgread16(cmsicap)
		var cfgre=cfgread16(cmsicap+2)
		var cfgr=cfgread32(cmsicap+4)
		var cfg=cfgread32(cmsicap+8)
		info(fmt('0x%04x',(cfgrea)))
		//info('0x'+(cfgrea).toString(16))
		info('MXID-MSI-X Identifiers')
		info('\tBits 07:00 Capability')
		info('\tBits 15:08 Next Pointer')
		//info('offset'+'  '+'0x'+(cmsicap+2).toString(16))
		info('offset'+'  '+fmt('0x%02x',(cmsicap+2)))
		info(fmt('0x%04x',(cfgre)))
		//info('0x'+(cfgre).toString(16))
		info('MXC-MSI-X Message Control')
		info('\tBits 10:00 Table Size')
		info('\tBits 13:11 Reserved')
		info('\tBit 14 Function Mask')
		info('\tBit 15 MSI-X Enable')
		//info('offset'+'  '+'0x'+(cmsicap+4).toString(16))
		info('offset'+'  '+fmt('0x%02x',(cmsicap+4)))
		info(fmt('0x%08x',(cfgr)))
		//info('0x'+(cfgr).toString(16))
		info('MTB-MSI-X Table Offset/Table BIR')
		info('\tBits 02:00 Table BIR')
		info('\tBits 31:03 Table Offset')
		//info('offset'+'  '+'0x'+(cmsicap+8).toString(16))
		info('offset'+'  '+fmt('0x%02x',(cmsicap+8)))
		info(fmt('0x%08x',(cfg)))
		//info('0x'+(cfg).toString(16))
		info('MPBA-MSI-X PBA Offset/PBA BIR')
		info('\tBits 02:00 PBA BIR')
		info('\tBits 31:03 PBA Offset')
		}
		
	} 
	else if(cmd=='rread'){
/* 		if(para1==undefined){	
			var print_st=''
			for(i = 0; i < 256;i=i+4){
				var rread=reg_read(i)
				print_st = print_st + "  "+ output.decimal2hex(rread,4)
				if(i!=0){
						if((i+4)%32==0){
					print_st= "Line:" + Math.floor(i/32)+'\t' + print_st 
                    info(print_st)	
                    print_st=''					
				}
				}	
		}	
		} */
		var rread=reg_read(para1)
		var rrea=reg_read(para1+4)
		if(para1==0){
			info(fmt('0x%08x',(rrea))+fmt('%04x',(rread)))
			info('CAP-Controller Capabilities')
			info('Bits 15:00 Maximum Queue Entries Supported')
			info('Bit 16    Contiguous Queues Required')
			info('Bits 18:17 Arbitration Mechanism Supported')
			info('Bits 23:19 Reserved')
			info('Bits 31:24 Timeout')
			info('\ta)0 to 1 after CC.EN transitions from 0 to 1;or b)1 to 0 after CC.EN transitions from 1 to 0')
			info('Bits 35:32 Doorbell Stride')
			info('Bit 36    NVM Subsystem Reset Supported')
			info('Bits 44:37 Command Sets Supported')
			info('Bits 47:45 Reserved')
			info('Bits 51:48 Memory Page Size Minimum')
			info('Bits 55:52 Memory Page Size Maximum')
			info('Bits 63:56 Reserved')
		}
		else if(para1==8){//0x08
		    //info('0x'+(rread).toString(16))
			info(fmt('0x%08x',(rread)))
			info('VS-Version')
			info('For 1.0 Compliant Controllers')
			info('Bits 07:00 Reserved')
			info('Bits 15:08 Minor Version Number:Indicates the minor version is 0')
			info('Bits 31:16 Major Version Number: Indicates the major version is 1')
			info('For 1.1 Compliant Controllers')
			info('Bits 07:00 Reserved')
			info('Bits 15:08 Minor Version Number:Indicates the minor version is 1')
			info('Bits 31:16 Major Version Number: Indicates the major version is 1')
			info('For 1.2 Compliant Controllers')
			info('Bits 07:00 Reserved')
			info('Bits 15:08 Minor Version Number:Indicates the minor version is 2')
			info('Bits 31:16 Major Version Number: Indicates the major version is 1')	
		}
		else if(para1==12){//0x0c
		    info(fmt('0x%08x',(rread)))
			info('INTMS-Interrupt Mask Set')
			info('Bit 31:00 Interrupt Vector Mask Set')
			
		}
		else if(para1==16){//0x10
		    info(fmt('0x%08x',(rread)))
			info('INTMC-Interrupt Mask Clear')
			info("Bits 31:00 Interrupt Vector Mask Clear")
		}
		else if(para1==20){//0x14
		    info(fmt('0x%08x',(rread)))
			info('CC-Controller Configuration')
			info('Bit 00    Enable:')
			info('\t\tset to 1,the controller shall process commands based on Submission Queue Tail doorbell writes')
			info('\t\tset to 0,the controller shall not process commands nor post completion queue entries to Completion Queues')
			info('Bits 03:01 Reserved')
			info('Bits 06:04 I/O Command Set Selected')
			info('Bits 10:07 Memory Page Size')
			info('Bits 13:11 Arbitration Mechanism Selected')
			info('Bits 15:14 Shutdown Notification')
			info('Bits 19:16 I/O Submission Queue Entry Size')
			info('Bits 23:20 I/O Completion Queue Entry Size')
			info('Bits 31:24 Reserved')
		}
		else if(para1==28){//0x1c
		    info(fmt('0x%08x',(rread)))
			info('CSTS-Controller Status')
			info('Bit 00    Ready')
			info('Bit 01    Controller Fatal Status')
			info('Bits 03:02 Shutdown Status')
			info('\t00b: Normal operation')
			info('\t01b: Shutdown processing occurring')
			info('\t10b: Shutdown processing complete')
			info('\t11b: Reserved')
			info('Bit 04    NVM Subsystem Reset Occurred')
			info('Bit 05    Processing Paused')
			info('Bits 31:06 Reserved')
		}
		else if(para1==32){//0x20
		    info(fmt('0x%08x',(rread)))
			info('NSSR-NVM Subsystem Reset')
			info('Bit 31:00 NVM Subsystem Reset Control')
		}
		else if(para1==36){//0x24
		    info(fmt('0x%08x',(rread)))
			info('AQA-Admin Queue Attributes')
			info('Bits 11:00 Admin Submission Queue Size')
			info('Bits 15:12 Reserved')
			info('Bits 27:16 Admin Completion Queue Size')
			info('Bits 31:28 Reserved')
		}
		else if(para1==40){//0x28
		    info(fmt('0x%08x',(rrea))+fmt('%08x',(rread)))
		    //info('0x'+(rrea).toString(16)+(rread).toString(16))
			info('ASQ-Admin Submission Queue Base Address')
			info('Bits 11:00 Reserved')
			info('Bits 63:12 Admin Submission Queue Base')
		}
		else if(para1==48){//0x30
		    info(fmt('0x%08x',(rrea))+fmt('%08x',(rread)))
		    //info('0x'+(rrea).toString(16)+(rread).toString(16))
			info('ACQ-Admin Completion Queue Base Address')
			info('Bits 11:00 Reserved')
			info('Bits 63:12 Admin Completion Queue Base')
		}
		else if(para1==56){//0x38
		    info(fmt('0x%08x',(rread)))
			info('CMBLOC-Controller Memory Buffer Location')
			info('Bits 2:0  Base Indicator Register')
			info('Bits 11:3 Reserved')
			info('Bits 31:12 Offset')
		}
		else if(para1==60){//0x3c
		    info(fmt('0x%08x',(rread)))
			info('CMBSZ-Controller Memory Buffer Size')
			info('Bit 0   Submission Queue Support')
			info('Bit 1   Completion Queue Support')
			info('Bit 2   PRP SGL List Support')
			info('Bit 3   Read Data Support')
			info('Bit 4   Write Data Support')
			info('Bits 7:5 Reserved')
			info('Bits 11:8 Size Units')
			info('\t0h:4KB')
			info('\t1h:64KB')
			info('\t2h:1MB')
			info('\t3h:16MB')
			info('\t4h:256KB')
			info('\t5h:4GB')
			info('\t6h:64GB')
			info('\t7h-Fh:Reserved')
			info('Bits 31:12 Size')
		}
		else 
			if(para1!=undefined){
				info('Parameter is not supported ')
			}
			
	}
    else if(cmd=='gctrl'){
			var print_st=''
			for(i = 0; i < 256;i=i+4){
				var gctrl=reg_read(i)
				print_st = print_st + "  "+fmt('%08x',(gctrl)) //output.decimal2hex(gctrl,4)
				if(i!=0){
					if((i+4)%16==0){
						if(i==12){
							print_st= "Offset:0" +'\t' + print_st 
							info(print_st)
							 print_st=''	
						}
						else{
							print_st= "Offset:" + Math.floor(i-12)+'\t' + print_st 
                            info(print_st)	
                            print_st=''		
						}
					
				}
				}	
		}	
		info('Bytes 07:00 Controller Capabilities')
		info('Bytes 0B:08 Version')
		info('Bytes 0F:0C Interrupt Mask Set')
		info('Bytes 13:10 Interrupt Mask Clear')
		info('Bytes 17:14 Controller Configuration')
		info('Bytes 1B:18 Reserved')
		info('Bytes 1F:1C Controller Status')
		info('Bytes 23:20 NVM Subsystem Reset')
		info('Bytes 27:24 Admin Queue Attributes')
		info('Bytes 2F:28 Admin Submission Queue Base Address')
		info('Bytes 37:30 Admin Completion Queue Base Address')
		info('Bytes 3B:38 Controller Memory Buffer Location')
		info('Bytes 3F:3C Controller Memory Buffer Size')
	}
	else if(cmd=='sirq'){
		
		if(para1 < 3){
			info('set_irq_type to : ' + IRQ_Str[Number(para1)] )
		 	shell.exec('cat /dev/null > /var/log/message')	
			set_irq_type(Number(para1))	
			cls()
			shell.exec('sudo rmmod nvme')
			shell.exec('sudo insmod ../../jdriver/nvme/nvme.ko')
			//var ret = shell.exec('grep -e "failed" /var/log/message')
			//console.log("ret = ",ret)
			//if (ret.output=="") info('set irq to '+ IRQ_Str[Number(process.argv[2])]+' sucess') 
			scan()	
			var irq = get_irq_type()
			if (Number(para1) == irq){
				info("Set IRQ Success, Now IRQ type is "+IRQ_Str[irq])
			}
			else
			{
				info("Set IRQ Failed, Now IRQ type is "+IRQ_Str[irq])
			}
				
        }
		else{
			info('irq type input must < 3')
        }
	}
	else if(cmd=='girq'){
		var irq = get_irq_type()
		info('Now IRQ type is  '+IRQ_Str[irq])
	}
	else{
		info('Command is undefined')
	}
info('end')

}

gui_cmd(process.argv[2],process.argv[3],process.argv[4],process.argv[5])
cls();
//history_dump("./history.log")	