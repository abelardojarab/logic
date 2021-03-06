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

`include "logic.svh"

/* Module: logic_axi4_stream_from_avalon_st
 *
 * Avalon-ST interface to AXI4-Stream interface bridge.
 *
 * Ports:
 *  aclk        - Clock.
 *  areset_n    - Asynchronous active-low reset.
 *  rx          - Avalon-ST interface.
 *  tx          - AXI4-Stream interface.
 */
module logic_axi4_stream_from_avalon_st #(
    int TDATA_BYTES = 1,
    int TDEST_WIDTH = 1,
    int TUSER_WIDTH = 1,
    int TID_WIDTH = 1,
    int EMPTY_WIDTH = (TDATA_BYTES >= 2) ? $clog2(TDATA_BYTES) : 1
) (
    input aclk,
    input areset_n,
    `LOGIC_MODPORT(logic_avalon_st_if, rx) rx,
    `LOGIC_MODPORT(logic_axi4_stream_if, tx) tx
);
    logic [TDATA_BYTES-1:0] strb;

    always_comb rx.ready = tx.tready;
    always_comb tx.tkeep = '1;
    always_comb tx.tuser = '0;
    always_comb tx.tdest = '0;

    always_ff @(posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            tx.tvalid <= '0;
        end
        else if (tx.tready) begin
            tx.tvalid <= rx.valid;
        end
    end

    always_comb begin
        strb = '1;

        if ($bits(tx.tstrb) > 1) begin
            for (bit [EMPTY_WIDTH-1:0] i = '0; i < rx.empty; ++i) begin
                strb = strb >> 1;
            end
        end
    end

    always_ff @(posedge aclk) begin
        if (tx.tready) begin
            tx.tlast <= rx.endofpacket;
            tx.tid <= rx.channel;
            tx.tstrb <= strb;
            tx.tdata <= rx.data;
        end
    end
endmodule
