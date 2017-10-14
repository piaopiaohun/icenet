import "DPI-C" function void network_tick
(
    input  bit     out_valid,
    output bit     out_ready,
    input  longint out_bits,

    output bit     in_valid,
    input  bit     in_ready,
    output longint in_bits,

    output longint macaddr
);

import "DPI-C" function void network_init(
    input string devname
);

module SimNetwork(
    input         clock,
    input         reset,

    input         net_out_valid,
    output        net_out_ready,
    input  [63:0] net_out_bits,

    output        net_in_valid,
    input         net_in_ready,
    output [63:0] net_in_bits,

    output [47:0] net_macAddr
);

    string devname;
    bit __out_ready;
    bit __in_valid;
    longint __in_bits;
    longint __macaddr;

    reg        __out_ready_reg;
    reg        __in_valid_reg;
    reg [63:0] __in_bits_reg;
    reg [47:0] __macaddr_reg;

    initial begin
        if ($value$plusargs("netdev=%s", devname) != 0) begin
            network_init(devname);
        end
    end

    /* verilator lint_off WIDTH */
    always @(posedge clock) begin
        if (reset) begin
            __out_ready = 0;
            __in_valid = 0;
            __in_bits = 0;

            __out_ready_reg <= 1'b0;
            __in_valid_reg <= 1'b0;
            __in_bits_reg <= 64'b0;
        end else begin
            network_tick(
                net_out_valid,
                __out_ready,
                net_out_bits,

                __in_valid,
                net_in_ready,
                __in_bits,

                __macaddr);

            __out_ready_reg <= __out_ready;
            __in_valid_reg <= __in_valid;
            __in_bits_reg <= __in_bits;
            __macaddr_reg <= __macaddr;
        end
    end

    assign net_out_ready = __out_ready_reg;
    assign net_in_valid = __in_valid_reg;
    assign net_in_bits = __in_bits_reg;
    assign net_macAddr = __macaddr_reg;

endmodule
