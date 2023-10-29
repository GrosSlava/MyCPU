BOARD=tangnano9k
FAMILY=GW1N-9C
DEVICE=GW1NR-LV9QN88PC6/I5

TOP_MODULE=CPU



all: Intermediate/out.fs

# Synthesis
Intermediate/synthesis.json: Src/*.v
	mkdir -p Intermediate
	yosys -p "read_verilog Src/*.v; synth_gowin -top ${TOP_MODULE} -json Intermediate/synthesis.json"

# Place and Route
Intermediate/bitstream.json: Intermediate/synthesis.json
	yowasp-nextpnr-gowin --json Intermediate/synthesis.json --freq 27 --write Intermediate/bitstream.json --device ${DEVICE} --family ${FAMILY} --cst ${BOARD}.cst

# Generate Bitstream
Intermediate/out.fs: Intermediate/bitstream.json
	gowin_pack -d ${FAMILY} -o Intermediate/out.fs Intermediate/bitstream.json

# Program Board
load: Intermediate/out.fs
	openFPGALoader -b ${BOARD} Intermediate/out.fs -f

# Remove build files
clean:
	rm -Rf ./Intermediate

.PHONY: load clean
