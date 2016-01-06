var sleep = require('sleep')
require('../../justice-cmd/command/nvme-cmd.js')
var logs =  require('../output/output.js');
var ref = require('ref')
var sprintf = require("sprintf-js").sprintf
var ref_struct = require('ref-struct')
var ref_array = require('ref-array')
var res = new Buffer(4096);
res.fill(0);
var w_buff = get_write_buffer()
var r_buff = get_read_buffer()

var char_t = ref.types.char
var uint8_t = ref.types.uint8
var uint16_t = ref.types.uint16
var uint32_t = ref.types.uint32
var uint64_t = ref.types.uint64

var nvme_admin = ref_struct({
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
})
var nvme_raw_io = ref_struct({
  opcode: uint8_t,
  flags: uint8_t,
  command_id: uint16_t,
  nsid: uint32_t,
  rsvd2: uint64_t,
  metadata: uint64_t,
  prp1: uint64_t,
  prp2: uint64_t,
  slba: uint64_t,
  length: uint16_t,
  control: uint16_t,
  dsmgmt: uint32_t,
  reftag: uint32_t,
  apptag: uint16_t,
  appmask: uint16_t,
})


var nvme_user_io = ref_struct({
  opcode: uint8_t,
  flags: uint8_t,
  control: uint16_t,
  nblocks: uint16_t,
  rsvd: uint16_t,
  metadata: uint64_t,
  addr: uint64_t,
  slba: uint64_t,
  dsmgmt: uint32_t,
  reftag: uint32_t,
  apptag: uint16_t,
  appmask: uint16_t,
})
scan()

console.log(process.env)
var testcount = process.env.TEST_CNT;
var output_len = 512;
for(var count =0;count < testcount;count++)
{
  info(sprintf("===========================test times:%d==================\n",count));
  var cmd_buff = process.env.CMDBYTE
  var temp = cmd_buff.split('\r')
  var temp2 =  temp[0]
  for (var i = 1;i<temp.length;i++)
  {
    temp2 = temp2.concat(temp[i])
  }
 
  temp3 = temp2.split(' ')

  var cnt = 1
  if(process.env.CMDTYPE == 'admin')
  {
      select(0,0,0);
      var admin = ref.alloc(nvme_admin);
      admin.fill(0x00);
      for (var i = 0;i<64;i++)
      {
          admin[i] = Number('0x'+temp3[i]);
      }

      var cmd = admin.deref();

      if(cmd.opcode == 6)
      {
         cmd.data_len = 4096
         output_len = 4096
      }
      else if(cmd.opcode == 2)
      {
      	 var fid = cmd.cdw10 & 0xff
      	 output_len = 512

      	 //console.log("fid:%d",fid)
      	 if (fid == 1)
      	 	cmd.data_len = 64
      	 else if(fid == 2)
      	 	cmd.data_len = 512
      	 else if(fid == 3)
      	 	cmd.data_len = 512
      }
      cmd.addr = ref.address(res)
      console.log(cmd)

      var ret = admin_passthru_async(cmd, 1)
      door_bell(0)
  } 
  else if(process.env.CMDTYPE == 'nvme') 
  {
      select(0,0,1);
      var cmd_alloc = ref.alloc(nvme_raw_io)
      cmd_alloc.fill(0);
      for (var i = 0;i<64;i++)
      {
          cmd_alloc[i] = Number('0x'+temp3[i])
      }

      var nvme_cmd = cmd_alloc.deref()
    
      if(nvme_cmd.opcode ==2 )
      {
          nvme_cmd.prp1 = ref.address(r_buff);
          output_len = (nvme_cmd.length+1)*512;
          //r_buff.copy(res)
          
      }
      else if(nvme_cmd.opcode ==1)
      {
          nvme_cmd.prp1 = ref.address(w_buff);
          output_len = 0;
      }

      console.log(nvme_cmd) 
    
      var ret = io_passthru_async(nvme_cmd, 2)
      door_bell(1)
  }


  while (cnt != 0) {
    var ret = retrieve()
    cnt -= ret.cnt
    for(var i = 0; i<ret.cnt;i++)
      {
          
          {
              
              info(sprintf(' cookie: %03d, result: %d, Status Code Type : 0x%02x, Status Code: 0x%02x',ret.results[i].cookie,ret.results[i].result,(ret.results[i].status&0x700)>>8,ret.results[i].status&0xff))

          }   
      }
  }

  var n_blk = output_len/512
  if (n_blk == 0)n_blk = 1
  if (n_blk >8 )n_blk = 8
  if(process.env.CMDTYPE == 'admin')
  logs.output_nline(res,n_blk,32)
  else if(process.env.CMDTYPE == 'nvme')
  {
     if(Number('0x'+temp3[0]) ==2 )
     {
        logs.output_nline(r_buff,n_blk,3)
     }
  } 
}

info('end')
cls();
