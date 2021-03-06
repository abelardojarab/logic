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

#include "basic_test.hpp"
#include "logic/axi4/stream/sequence.hpp"

basic_test::basic_test(
        const uvm::uvm_component_name& name) :
    logic::axi4::stream::test{name}
{
    scv_random::set_global_seed(100);
}

basic_test::~basic_test() { }

void basic_test::build_phase(uvm::uvm_phase& phase) {
    logic::axi4::stream::test::build_phase(phase);

    m_sequence->reset_repeats->reset_distribution();
    m_sequence->reset_repeats->keep_only(1);

    m_sequence->reset_duration->reset_distribution();
    m_sequence->reset_duration->keep_only(1, 4);

    m_sequence->reset_idle->reset_distribution();
    m_sequence->reset_idle->keep_only(0, 3);

    m_sequence->rx_number_of_packets->reset_distribution();;
    m_sequence->rx_number_of_packets->keep_only(1, 4);

    m_sequence->rx_repeats->reset_distribution();
    m_sequence->rx_repeats->keep_only(8);

    m_sequence->rx_packet_length->reset_distribution();
    m_sequence->rx_packet_length->keep_only(256, 512);

    m_sequence->rx_idle_scheme->reset_distribution();
    m_sequence->rx_idle_scheme->keep_only(0, 2);

    m_sequence->tx_idle_scheme->reset_distribution();
    m_sequence->tx_idle_scheme->keep_only(3, 4);
}
