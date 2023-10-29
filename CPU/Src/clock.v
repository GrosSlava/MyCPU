
`default_nettype none



/**
	Generate the system clock based on in incoming clock. 
	Stop clock if halt signal is high.
*/
module Clock
(
	input wire i_clk,
	input wire i_halt,
	output reg o_clk
);

localparam WAIT_TIME = 130000;

reg [23:0] l_clockCounter;

initial begin
	l_clockCounter = 0;
	o_clk = 1'b0;
end

always @(posedge i_clk) begin
	if( i_halt == 1'b0 ) begin
		l_clockCounter = l_clockCounter + 1;
		if( l_clockCounter == WAIT_TIME ) begin
			o_clk <= ~o_clk;
			l_clockCounter <= 0;
		end
	end 
	else begin
		o_clk <= 1'b0;
	end
end

endmodule // Clock
