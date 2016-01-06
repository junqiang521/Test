/*############################################################
 ***Purpuse: The script to verify justice phase6 command
 Here is the phase6 commands:
- pcie_ps(PCIe power state D0 or D3hot, PMCAP+4h))
- pcie_max_pl( PCIe max payload size, PXCAP+8h)
- pcie_aspm(L0, L0s, L1)
- NVMe controller reset(CC of Controller Register)
- NVMe shutdown(CC of Controller Register)
- AMS(Arbitration mechanism Selected) setting

***Command:
         sudo node phase6.js ps ////for power state 
         sudo node phase6.js pl //for max payload
         sudo node phase6.js aspm //for active state power state
         sudo node phase6.js reset //for reset 
         sudo node phase6.js shutdown // for shut down
         sudo node phase6.js ams //for Arbitration mechanism Selected
         sudo node phase6.js all // for all phase6 commands
         
 ***Precondition:
 -PCIE power adpater has been installed
 -Diver and command interface has been updated and has been installed
 -The program for power cycle "pcie_pa GDB.bin" has been copy in "/home/justice"
 -Bin file for power adpater has been load successfully
 -The power adpater can work properly with the program "pcie_pa"
 
*** Date: 2015/11/30
***Author: Lili Wu
***Note:
    -Reset and shutdown implementation and verifying need discusstion
     -AMS has not been done, move to the next stage
##############################################################*/

//require('../command/nvme-cmd.js')
require('../../justice-cmd/command/nvme-cmd.js')
var assert = require('assert')
var sleep = require('sleep')
var printf = require('printf')

var w_slba = 0
var w_nblocks = 8
var w_buf_offset = 0    
var r_slba = 0
var r_nblocks = 8
var r_buf_offset = 0

var cnt = 8
var ctrl = 0
var ns = 0
var nqueue = 1
var base_data = 0x55

var pw_on = "/home/justice/pcie_pa -p /dev/sdb on"
var pw_off = "/home/justice/pcie_pa -p /dev/sdb off"
     
function write() {
    var i
       
    for (i = 0; i != cnt; i++) { 
      write_async(w_slba, w_nblocks, i, w_buf_offset)
      w_slba += w_nblocks
      w_buf_offset += w_nblocks * 512
    }
    door_bell()
    
    while (cnt != 0) {
      var ret = retrieve()
      cnt -= ret.cnt
    }  
}

function read() {
    var i

    for (i = 0; i != cnt; i++) { 
      read_async(r_slba, r_nblocks, i, r_buf_offset)
      r_slba += r_nblocks
      r_buf_offset += r_nblocks * 512
    }
    door_bell()
    
    while (cnt != 0) {
      var ret = retrieve()
      cnt -= ret.cnt
    }
  
}

function compare() {
    var len = r_nblocks * 512 * cnt
    var r_offset = 0
    var w_offset = 0
    var ret = cmp(len, r_offset, w_offset)
    if (ret) {
        info('Data Miscompare: Read Data != Write data') 
    }
    info('Read and Write Test: PASS')
    return ret
}

function power_state(negative) {
    var pmcap = cfgpmcap()
    info('Reading the currect power state')
    var ps = cfgread8(pmcap + 0x4)
    var cur_ps = ps & 0x03
    switch(cur_ps) {
      case 0x0:
        info('Current power state: D0')
        break
      case 0x01:
        info('Current power state: D1')
        break
      case 0x02:
        info('Current power state: D2')
        break
      case 0x03:
        info('Current power state: D3 hot')
        break
      default:
        info('invalid value for power state')
     }
    
    info('Checking register I/O before setting: Controller Capabilities')
    var ctrl_cap=reg_read(0)
    info(printf('%04x',(ctrl_cap)))
    var power_cap = cfgread16(pmcap + 0x2)    
    var d2s = (power_cap & 0x0400)
    var d1s = (power_cap & 0x0200)
    if (d1s === 0x0200) {
      info('Setting the device to D1 power state')
      ret = set_pw_state(0x01)
      assert(ret == 0)
      var ps = cfgread8(pmcap + 0x4)
      assert((ps & 0x03) == 0x01)
      write()
      read()
      var ret = compare()
      assert(ret == 0)
    }
    else
      info('D1 power state is not support') 

    if (d2s === 0x0400) {
      info('Setting the device to D2 power state')
      ret = set_pw_state(0x02)
      assert(ret == 0)
      var ps = cfgread8(pmcap + 0x4)
      assert((ps & 0x03) == 0x02)
      write()
      read()
      var ret = compare()
      assert(ret == 0)
    }
    else
      info('D2 power state is not support') 

    info('Setting the device to D3 power state')
    var ret = set_pw_state(0x03)
    assert(ret == 0)
    var ps = cfgread8(pmcap + 0x4)
    assert((ps & 0x03) == 0x03)
    info('Checking register I/O before setting D3: Controller Capabilities')
    var ctrl_cap=reg_read(0)
    info(printf('%04x',(ctrl_cap)))
    assert(ctrl_cap == 4294967295) //the value read should be ff after setting D3
    info('Setting the device to D0 power state')
    var ret = set_pw_state(0x00)
    assert(ret == 0)
    var ps = cfgread8(pmcap + 0x4)
    assert((ps & 0x03) == 0x00)
    write()
    read()
    var ret = compare()
    assert(ret == 0)
    info('Verifying normal flow of power state:PASS')
    
    if (negative === 1) {
      info('Negative test for power state')
      for (var i=0; i<4; i++) {
          var ret = set_pw_state(i)
          var ret_str = (ret===1)?" Fail ":" PASS "
          info('setting power state: D' + i + ' result: ' + ret_str)
         }
     
      info('Temp solution: Back to D0 before power off')
      var ret = set_pw_state(0x00) //temp solution: when in D3, doing power off and power on, the base address is always 0
      assert(ret == 0)
      power_off(pw_off)
      info('sleep 5s')
      sleep.sleep(5)
      power_on(pw_on)
      info('Reading the power state after power on')
      var pmcap = cfgpmcap()
      var ps = cfgread8(pmcap + 0x4)
      assert((ps & 0x03) == 0x00)
      write()
      read()
      var ret = compare()
      assert(ret == 0)
      info('Negative test for power state: PASS')
    }
}

function max_payload() {
    var i

    for (i = 0; i < 6; i++) { 
      var set_payload = 128 * (1 << i)
      var ret = set_maxpayload(i)
      info("Platform Max Pay Load Capacity: " + ret)
      info('Setting max payload: ' + set_payload)
      if (set_payload <= ret)
        info('Current Pay Load=' + set_payload)
      else
        info('Current Pay Load=' + ret)
      write()
      read()
      var ret = compare()
      assert(ret == 0)
    }
      info('Verifying max payload : PASS')
}

function aspm() {
   var i
   var cap = pcie_aspm_cap()
   info('Aspm Capacity: ' + cap)
   var cur_aspm = pcie_aspm_cap()
   info('Current Aspm: ' + cur_aspm) 
   var cur_aspm = pcie_get_aspm()
   for (i = 0; i < 4; i++) {
      if (i <= cap) {
        info('Setting Aspm Level: ' + i)
        ret = set_aspm(i)
        assert(ret == 0)
        var new_aspm = pcie_get_aspm()
        assert(new_aspm  == i)
        write()
        read()
        var ret = compare()
        assert(ret == 0)
      }
      else {
        ret = set_aspm(i)
        assert(ret == 1)
      }
   }
   info('Verifying Pcie Aspm : PASS')
}

function ams() {
   info('Plan to add AMS in the future')
}

function reset() {
    /*1. Reading the value: the CC.EN controller register field
      2. Doing controller reset:Configure the NVMe Host to write a
         value of 0 to the CC.EN controller register field
      3. Verify that the value of the CC.SHN field is set to its default
         value of 00b (no notification) after a Con-troller Level Reset*/
    info('Controller Reset')
    ctrl_reset()
    write()
    read()
    var ret = compare()
    assert(ret == 0)
    info('Verifying Controller Reset:PASS')
    
    /*Before doing the function,checking the value:
      1. FLRC) field (bit 28) of PXDCAP(offset PXCAP + 4h)should be 1
      2. bit 15 of PXDC (offset PXCAP + 8h) should be 0
      3. Doing FLR:Configure the NVMe Host to write a value of 1 to the
         Initiate Function Level Reset field of the PXDC PCI Express register*/
    info('Function Level Reset')
    pci_flr()
    write()
    read()
    var ret = compare()
    assert(ret == 0)
    info('Verifying Function Level Reset:PASS')
}

function shutdown() {
    //read the CC.SHN field (bits 15:14) of the NVMe Controller
    //read the CSTS.SHST field (bits 03:02) of the NVMe Controller
    //read the CSTS.SHST field of the NVMe Controller.
    shutdown_reset()
    write()
    read()
    var ret = compare()
    assert(ret == 0)
    info('Verifying shutdown reset:PASS')
}


function phase6_cmd(cmd) {
    var wbuf = get_write_buffer()
    var rbuf = get_read_buffer()
    
    for(i = 0; i < w_nblocks * 512 * cnt; i++)
        wbuf[i] = base_data
    
     //for(i = 0; i < w_nblocks * 512 * cnt; i++)
     //   info(wbuf[i])

    //var buff = new Buffer(512)   
    //for(i = 0; i < 512; i++)
    //    buff[i] = rbuf[i]
        
    select(ctrl, ns, nqueue)
    scan()
    
    switch(cmd) {
    case 'ps':
      info('Verifying PCIe power state D0 or D3hot')
      power_state(negative=0)
      break
    case 'pl':
      info('Verifying PCIe max payload size')
      max_payload()
      break
    case 'aspm':
      info('Verifying pcie_aspm')
      aspm()
      break
    case 'reset':
      info('Verifying NVMe controller reset')
      reset()
      break
    case 'shutdown':
      info('Verifying NVMe shutdown')
      shutdown()
      break
    case 'ams':
      info('Note: Arbitration mechanism Selected Not Done, move to the next stage')
      break
    case 'all':
      info('Verifying all the commands that has been done in phase 6')
      power_state(negative=0)
      info('Verifying PCIe max payload size')
      max_payload()
      info('Verifying pcie_aspm')
      aspm()
      info('Verifying NVMe controller reset')
      reset()
      info('Verifying NVMe shutdown')
      shutdown()
      info('Note: Arbitration mechanism Selected Not Done, move to the next stage')
      ams()
      info('Verifying all the commands that has been done in phase 6: PASS')
      break
    default:
        info('Invalid Cmd, the proper parameters should be ps or pl or aspm or reset or shutdown or ams or all')
}
    cls()
}

//Running the script
phase6_cmd(process.argv[2])

/*
if (require.main === module) 
{
  info('Debugging for this script')    
}*/
