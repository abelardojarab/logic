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

/* Module: logic_axi4_lite_write_aligned_main
 *
 * Align write address channel and write data channel.
 *
 * Ports:
 *  aclk        - Clock.
 *  areset_n    - Asynchronous active-low reset.
 *  slave       - AXI4-Lite slave interface.
 *  master      - AXI4-Lite master interface.
 */
module logic_axi4_lite_write_aligned_main #(
    int DATA_BYTES = 4,
    int ADDRESS_WIDTH = 1
) (
    input aclk,
    input areset_n,
    `LOGIC_MODPORT(logic_axi4_lite_if, slave) slave,
    `LOGIC_MODPORT(logic_axi4_lite_if, master) master
);
    enum logic [1:0] {
        FSM_IDLE,
        FSM_WAIT_FOR_DATA,
        FSM_WAIT_FOR_ADDRESS
    } fsm_state;

    logic data_ready;
    logic address_ready;

    always_comb data_ready = slave.wvalid && master.wready;
    always_comb address_ready = slave.awvalid && master.awready;

    always_comb slave.arready = master.arready;
    always_comb master.bready = slave.bready;
    always_comb master.rready = slave.rready;

    always_ff @(posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            fsm_state <= FSM_IDLE;
        end
        else begin
            unique case (fsm_state)
            FSM_IDLE: begin
                if (address_ready && !data_ready) begin
                    fsm_state <= FSM_WAIT_FOR_DATA;
                end
                else if (!address_ready && data_ready) begin
                    fsm_state <= FSM_WAIT_FOR_ADDRESS;
                end
            end
            FSM_WAIT_FOR_DATA: begin
                if (address_ready && data_ready) begin
                    fsm_state <= FSM_IDLE;
                end
            end
            FSM_WAIT_FOR_ADDRESS: begin
                if (address_ready && data_ready) begin
                    fsm_state <= FSM_IDLE;
                end
            end
            default: begin
                fsm_state <= FSM_IDLE;
            end
            endcase
        end
    end

    always_comb begin
        unique case (fsm_state)
        FSM_IDLE: begin
            if (address_ready && !data_ready) begin
                slave.awready = 1'b0;
            end
            else begin
                slave.awready = master.awready;
            end
        end
        FSM_WAIT_FOR_DATA: begin
            slave.awready = 1'b0;
        end
        default: begin
            slave.awready = master.awready;
        end
        endcase
    end

    always_comb begin
        unique case (fsm_state)
        FSM_IDLE: begin
            if (!address_ready && data_ready) begin
                slave.wready = 1'b0;
            end
            else begin
                slave.wready = master.wready;
            end
        end
        FSM_WAIT_FOR_ADDRESS: begin
            slave.wready = 1'b0;
        end
        default: begin
            slave.wready = master.wready;
        end
        endcase
    end

    always_ff @(posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            master.awvalid <= 1'b0;
        end
        else if (master.awready) begin
            master.awvalid <= slave.awvalid && slave.wvalid && master.wready;
        end
    end

    always_ff @(posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            master.wvalid <= 1'b0;
        end
        else if (master.wready) begin
            master.wvalid <= slave.wvalid && slave.awvalid && master.awready;
        end
    end

    always_ff @(posedge aclk) begin
        if (master.wready) begin
            master.wstrb <= slave.wstrb;
            master.wdata <= slave.wdata;
        end
    end

    always_ff @(posedge aclk) begin
        if (master.awready) begin
            master.awprot <= slave.awprot;
            master.awaddr <= slave.awaddr;
        end
    end

    always_ff @(posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            slave.bvalid <= 1'b0;
        end
        else if (slave.bready) begin
            slave.bvalid <= master.bvalid;
        end
    end

    always_ff @(posedge aclk) begin
        if (slave.bready) begin
            slave.bresp <= master.bresp;
        end
    end

    always_ff @(posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            master.arvalid <= 1'b0;
        end
        else if (master.arready) begin
            master.arvalid <= slave.arvalid;
        end
    end

    always_ff @(posedge aclk) begin
        if (master.arready) begin
            master.araddr <= slave.araddr;
            master.arprot <= slave.arprot;
        end
    end

    always_ff @(posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            slave.rvalid <= 1'b0;
        end
        else if (slave.rready) begin
            slave.rvalid <= master.rvalid;
        end
    end

    always_ff @(posedge aclk) begin
        if (slave.rready) begin
            slave.rresp <= master.rresp;
            slave.rdata <= master.rdata;
        end
    end
endmodule
