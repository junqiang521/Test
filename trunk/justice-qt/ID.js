require('../justice-cmd/command/nvme-cmd.js')

var assert = require('assert')
var ref = require('ref')
var ref_struct = require('ref-struct')
var ref_array = require('ref-array')
var ioctl = require('ioctl')
var sleep = require('sleep');


var char_t = ref.types.char
var uint8_t = ref.types.uint8
var uint16_t = ref.types.uint16
var uint32_t = ref.types.uint32
var uint64_t = ref.types.uint64

var nvme_admin_cmd = ref_struct({
  opcode: uint8_t,
  flags: uint8_t,
  rsvd1: uint16_t,
  nsid: uint32_t,
  cdw2: uint32_t,
  cdw3: uint32_t,
  metadata: uint64_t,
  addr: uint64_t,
  metadata_len: uint32_t,
  data_len: uint32_t,
  cdw10: uint32_t,
  cdw11: uint32_t,
  cdw12: uint32_t,
  cdw13: uint32_t,
  cdw14: uint32_t,
  cdw15: uint32_t,
  timeout: uint32_t,
  result: uint32_t,
})

var NVME_IOCTL_ADMIN_CMD = 0xc0484e41

var nvme_id_power_state = ref_struct
({
  max_power: uint16_t,
  rsvd2: uint8_t,
  flags: uint8_t,
  entry_lat: uint32_t,
  exit_lat: uint32_t,
  read_tput: uint8_t,
  read_lat: uint8_t,
  write_tput: uint8_t,
  write_lat: uint8_t,
  idle_power: uint16_t,
  idle_scale: uint8_t,
  rsvd19: uint8_t,
  active_power: uint16_t,
  active_work_scale: uint8_t,
  rsvd23: ref_array(uint8_t, 9),
})

var nvme_id_ctrl = ref_struct
({
  vid: uint16_t,
  ssvid: uint16_t,
  
  sn: ref_array(char_t, 20),         // Serial
  mn: ref_array(char_t, 40),       //  Model
  fr: ref_array(char_t, 8),            //  firmware

  rab: uint8_t,
  ieee: ref_array(uint8_t, 3),
  cmic: uint8_t,
  mdts: uint8_t,
  cntlid: uint16_t,
  ver: uint32_t,
  rtd3r: uint32_t,
  rtd3e: uint32_t,
  oaes: uint32_t,
  rsvd96: ref_array(uint8_t, 160),
  oacs: uint16_t,
  acl: uint8_t,
  aerl: uint8_t,
  frmw: uint8_t,
  lpa: uint8_t,
  elpe: uint8_t,
  npss: uint8_t,
  avscc: uint8_t,
  apsta: uint8_t,
  wctemp: uint16_t,
  cctemp: uint16_t,
  mtfa: uint16_t,
  hmpre: uint32_t,
  hmmin: uint32_t,
  tnvmcap: ref_array(uint8_t, 16),
  unvmcap: ref_array(uint8_t, 16),
  rpmbs: uint32_t,
  rsvd316: ref_array(uint8_t, 196),
  sqes: uint8_t,
  cqes: uint8_t,
  rsvd514: ref_array(uint8_t, 2),
  nn: uint32_t,
  oncs: uint16_t,
  fuses: uint16_t,
  fna: uint8_t,
  vwc: uint8_t,
  awun: uint16_t,
  awupf: uint16_t,
  nvscc: uint8_t,
  rsvd531: uint8_t,
  acwu: uint16_t,
  rsvd534: ref_array(uint8_t, 2),
  sgls: uint32_t,
  rsvd540: ref_array(uint8_t, 1508),
  psd: ref_array(nvme_id_power_state, 32),
  vs: ref_array(uint8_t, 1024),
})

var nvme_lbaf =  ref_struct
({
  ms: uint16_t,
  ds: uint8_t,
  rp: uint8_t,
})


var nvme_id_ns = ref_struct
({
    nsze: uint64_t,                    // capacuty
    ncap: uint64_t,                    // capacuty
    nuse: uint64_t,
    nsfeat: uint8_t,
    nlbaf: uint8_t,
    flbas: uint8_t,
    mc: uint8_t,
    dpc: uint8_t,
    dps: uint8_t,
    nmic: uint8_t,
    rescap: uint8_t,
    fpi: uint8_t,
    rsvd33: uint8_t,
    nawun: uint16_t,
    nawupf: uint16_t,
    nacwu: uint16_t,
    nabsn: uint16_t,
    nabo: uint16_t,
    nabspf: uint16_t,
    rsvd46: uint16_t,
    nvmcap: ref_array(uint8_t, 16),
    rsvd64: ref_array(uint8_t, 40),
    nguid: ref_array(uint8_t, 16),
    eui64: ref_array(uint8_t, 8),
    lbaf: ref_array(nvme_lbaf, 16),
    rsvd192: ref_array(uint8_t, 192),
    vs: ref_array(uint8_t, 3712),

})

var nvme_ns_list = ref_array(uint32_t, 1024)

var cmd_alloc = ref.alloc(nvme_admin_cmd)

var nvme_admin_identify = 0x6

function nvme_identify ( fd ,  nsid ,  cns )
 {
   var res
  if (cns == 0)
    res = ref.alloc( nvme_id_ns )
  if (cns == 1)
    res = ref.alloc( nvme_id_ctrl )
  if (cns == 2)
    res = ref.alloc( nvme_ns_list )

  assert(res.length == 4096)

  cmd_alloc.fill( 0x00 )
  var cmd = cmd_alloc.deref()
  cmd.opcode = nvme_admin_identify
  cmd.nsid = nsid
  cmd.addr = ref.address(res)
  cmd.data_len = 4096
  cmd.cdw10 = cns
  var ret = ioctl( fd ,  NVME_IOCTL_ADMIN_CMD ,  cmd_alloc )
  assert(ret == 0)

  return res.deref()
}

exports.nvme_identify = nvme_identify

info('start')

if (require.main === module) 
{

  var fs = require('fs')
  //var fd = fs.openSync('/dev/nvme0n1', 'r+')
  
  // Modify by Alwin 0824 for two PCIE socket case
  if( process.argv[3] == 0 )
    var fd = fs.openSync('/dev/nvme0n1', 'r+')
  else if( process.argv[3] == 1 )
    var fd = fs.openSync('/dev/nvme1n1', 'r+')
  else if( process.argv[3] == 2 )
    var fd = fs.openSync('/dev/nvme2n1', 'r+')
  else if( process.argv[3] == 3 )
    var fd = fs.openSync('/dev/nvme3n1', 'r+')
  else
   {  
     message = "Get device error , argv[ 3 ]:" + process.argv[3]; 
     info( 'msg:' + message )
   }
 
 
  var id_ctrl_result = nvme_identify( fd ,  0 , 1 )  // conyrol   

// Model 40 char 
 
 var Model  =  String.fromCharCode(    id_ctrl_result.mn[ 0 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 1 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 2 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 3 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 4 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 5 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 6 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 7 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 8 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 9 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 10 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 11 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 12 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 13 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 14 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 15 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 16 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 17 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 18 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 19 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 20 ] )  
+ String.fromCharCode(    id_ctrl_result.mn[ 21 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 22 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 23 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 24 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 25 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 26 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 27 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 28 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 29 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 30 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 31 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 32 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 33 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 34 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 35 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 36 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 37 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 38 ]  )  
+ String.fromCharCode(    id_ctrl_result.mn[ 39 ]  ) ; 

   message = "Model : "  + Model;
   info( 'msg:' + message )

 // --------------------------------------------------

// Firmware 8 char 
  var FW  =  String.fromCharCode(    id_ctrl_result.fr[ 0 ]  )  
+ String.fromCharCode(    id_ctrl_result.fr[ 1 ]  )  
+ String.fromCharCode(    id_ctrl_result.fr[ 2 ]  )  
+ String.fromCharCode(    id_ctrl_result.fr[ 3 ]  )  
+ String.fromCharCode(    id_ctrl_result.fr[ 4 ]  )  
+ String.fromCharCode(    id_ctrl_result.fr[ 5 ]  )  
+ String.fromCharCode(    id_ctrl_result.fr[ 6 ]  )  
+ String.fromCharCode(    id_ctrl_result.fr[ 7 ]  )  ;

  message = "Firmware ver : " + FW;
   info( 'msg:' + message )

// ---------------------------------------------------
// Serial number  20 char 

  var Serial  =  String.fromCharCode(    id_ctrl_result.sn[ 0 ]  )  
+ String.fromCharCode( id_ctrl_result.sn[ 1 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 2 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 3 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 4 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 5 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 6 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 7 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 8 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 9 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 10 ]) 
+ String.fromCharCode( id_ctrl_result.sn[ 11 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 12 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 13 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 14 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 15 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 16 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 17 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 18 ] )  
+ String.fromCharCode( id_ctrl_result.sn[ 19 ] ); 

   message = "Serial number : " +Serial;
   info( 'msg:' + message )

//----------------------------------------------------------------------------------------

  var id_ns_result = nvme_identify(fd, 1, 0)     // namespace

   var  MAXLBA = "MAXLBA : " +  id_ns_result.nsze;
   message = MAXLBA.toString() ;
   info( 'msg:' + message )

//----------------------------------------------------------------------------------------

  var ns_list = nvme_identify(fd, 1, 2)
  fs.close(fd)

  //  var xbuf = new ref.alloc(nvme_id_ns)
  //  var xbuf = new ref.alloc(nvme_id_ctrl)
  //  var xbuf = new ref.alloc(nvme_ns_list)
  
  // var  xbuf = nvme_id_ctrl;
  // console.log( xbuf.fr   );
 
  info('end')


}





















































































































































