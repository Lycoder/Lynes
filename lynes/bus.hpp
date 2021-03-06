#pragma once

#include "global.hpp"

#include "devices/ppu/ppu.hpp"
#include "devices/apu/apu.hpp"
#include "devices/input.hpp"
#include "devices/cart.hpp"
#include "devices/ram.hpp"
#include "devices/dma/dma.hpp"

#define IN_RANGE(l, h) ((addr >= l) && (addr <= h))

namespace nes {
    // CPU Bus
    namespace bus {
        u8 read(u16 addr) {
            //_log(debug, "read addr=%04x", addr);
            if (IN_RANGE(RAM_BEGIN  , RAM_END  )) { return ram::read(addr); }
            if (IN_RANGE(PPU_BEGIN  , PPU_END  )) { return ppu::read(addr); }
            if (IN_RANGE(APU_BEGIN  , APU_END-2)) { return apu::read(addr); }
            if (IN_RANGE(CART_BEGIN , CART_END )) { return cart::read(addr); }
            if (IN_RANGE(INPUT_BEGIN, INPUT_END)) { return input::read(addr); }

            return 0x0;
        }

        void write(u16 addr, u8 value) {
            if (IN_RANGE(RAM_BEGIN  , RAM_END      )) { ram::write(addr, value); return; }
            if (IN_RANGE(PPU_BEGIN  , PPU_END      )) { ppu::write(addr, value); return; }
            if (IN_RANGE(APU_BEGIN  , 0x400f       )) { apu::write(addr, value); return; }
            //if (IN_RANGE(0x4017     , 0x4017       )) { apu::write(addr, value); return; }
            if (IN_RANGE(INPUT_BEGIN, INPUT_END    )) { input::write(addr, value); return; }
            if (IN_RANGE(CART_BEGIN , CART_END     )) { cart::write(addr, value); return; }
            if (IN_RANGE(DMA_BEGIN  , DMA_END      )) { dma::write(addr, value); return; }
        }
    }
}

#undef IN_RANGE