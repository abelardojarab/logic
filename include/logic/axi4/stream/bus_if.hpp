/* Copyright 2018 Tymoteusz Blazejczyk
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LOGIC_AXI4_STREAM_BUS_IF_HPP
#define LOGIC_AXI4_STREAM_BUS_IF_HPP

#include "logic/utils.hpp"
#include "logic/axi4/stream/defines.hpp"
#include "logic/axi4/stream/bus_if_base.hpp"

#include <systemc>

#include <cstdint>

namespace logic {
namespace axi4 {
namespace stream {

template<std::size_t TDATA_BYTES = LOGIC_AXI4_STREAM_TDATA_BYTES,
    std::size_t TID_WIDTH = LOGIC_AXI4_STREAM_TID_WIDTH,
    std::size_t TDEST_WIDTH = LOGIC_AXI4_STREAM_TDEST_WIDTH,
    std::size_t TUSER_WIDTH = LOGIC_AXI4_STREAM_TUSER_WIDTH>
class bus_if : public bus_if_base {
public:
    using tid_type = typename utils::bits<TID_WIDTH>::type;
    using tdata_type = typename utils::bits<TDATA_BYTES * 8>::type;
    using tstrb_type = typename utils::bits<TDATA_BYTES>::type;
    using tkeep_type = typename utils::bits<TDATA_BYTES>::type;
    using tdest_type = typename utils::bits<TDEST_WIDTH>::type;
    using tuser_type = typename utils::bits<TUSER_WIDTH>::type;

    sc_core::sc_signal<tid_type> tid{"tid"};
    sc_core::sc_signal<tdata_type> tdata{"tdata"};
    sc_core::sc_signal<tstrb_type> tstrb{"tstrb"};
    sc_core::sc_signal<tkeep_type> tkeep{"tkeep"};
    sc_core::sc_signal<tdest_type> tdest{"tdest"};
    sc_core::sc_signal<tuser_type> tuser{"tuser"};

    bus_if(const sc_core::sc_module_name& name) :
        bus_if_base{name}
    { }

    virtual void trace(sc_core::sc_trace_file* trace_file) const override {
        bus_if_base::trace(trace_file);

        if (trace_file) {
            sc_core::sc_trace(trace_file, tid, tid.name());
            sc_core::sc_trace(trace_file, tdata, tdata.name());
            sc_core::sc_trace(trace_file, tstrb, tstrb.name());
            sc_core::sc_trace(trace_file, tkeep, tkeep.name());
            sc_core::sc_trace(trace_file, tuser, tuser.name());
        }
    }

    virtual std::size_t size() const noexcept override {
        return TDATA_BYTES;
    }

    virtual std::uint8_t get_tdata(std::size_t offset) const override {
        return utils::get_uint8<8 * TDATA_BYTES>(tdata.read(), 8 * offset);
    }

    virtual void set_tdata(std::size_t offset, std::uint8_t value) override {
        utils::set<8 * TDATA_BYTES>(m_tdata, 8 * offset, value);
        tdata.write(m_tdata);
    }

    virtual bool get_tkeep(std::size_t offset) const override {
        return utils::get_bool<TDATA_BYTES>(tkeep.read(), offset);
    }

    virtual void set_tkeep(std::size_t offset, bool value) override {
        utils::set<TDATA_BYTES>(m_tkeep, offset, value);
        tkeep.write(m_tkeep);
    }

    virtual bool get_tstrb(std::size_t offset) const override {
        return utils::get_bool<TDATA_BYTES>(tstrb.read(), offset);
    }

    virtual void set_tstrb(std::size_t offset, bool value) override {
        utils::set<TDATA_BYTES>(m_tstrb, offset, value);
        tstrb.write(m_tstrb);
    }

    virtual bitstream get_tid() const override {
        bitstream bits(TID_WIDTH);
        for (std::size_t i = 0; i < TID_WIDTH; ++i) {
            bits[i] = utils::get_bool<TID_WIDTH>(tid.read(), i);
        }
        return bits;
    }

    virtual void set_tid(const bitstream& bits) override {
        auto size = (bits.size() < TID_WIDTH)
            ? bits.size() : TID_WIDTH;

        tid_type value{};
        for (std::size_t i = 0; i < size; ++i) {
            utils::set<TID_WIDTH>(value, i, bits[i]);
        }
        tid.write(value);
    }

    virtual bitstream get_tdest() const override {
        bitstream bits(TDEST_WIDTH);
        for (std::size_t i = 0; i < TDEST_WIDTH; ++i) {
            bits[i] = utils::get_bool<TDEST_WIDTH>(tdest.read(), i);
        }
        return bits;
    }

    virtual void set_tdest(const bitstream& bits) override {
        auto size = (bits.size() < TDEST_WIDTH)
            ? bits.size() : TDEST_WIDTH;

        tdest_type value{};
        for (std::size_t i = 0; i < size; ++i) {
            utils::set<TDEST_WIDTH>(value, i, bits[i]);
        }
        tdest.write(value);
    }

    virtual bitstream get_tuser() const override {
        bitstream bits(TUSER_WIDTH);
        for (std::size_t i = 0; i < TUSER_WIDTH; ++i) {
            bits[i] = utils::get_bool<TUSER_WIDTH>(tuser.read(), i);
        }
        return bits;
    }

    virtual void set_tuser(const bitstream& bits) override {
        auto size = (bits.size() < TUSER_WIDTH)
            ? bits.size() : TUSER_WIDTH;

        tuser_type value{};
        for (std::size_t i = 0; i < size; ++i) {
            utils::set<TUSER_WIDTH>(value, i, bits[i]);
        }
        tuser.write(value);
    }

    virtual ~bus_if() override { }
private:
    tdata_type m_tdata{};
    tkeep_type m_tkeep{};
    tstrb_type m_tstrb{};
};

}
}
}

#endif /* LOGIC_AXI4_STREAM_BUS_IF_HPP */
