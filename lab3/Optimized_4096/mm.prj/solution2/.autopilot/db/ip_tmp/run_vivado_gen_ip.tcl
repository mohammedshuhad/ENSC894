create_project prj -part xcu50-fsvh2104-2-e -force
set_property target_language verilog [current_project]
set vivado_ver [version -short]
set COE_DIR "../../syn/verilog"
source "/local-scratch/localhome/msa325/ensc894/lab3/Optimized_4096/mm.prj/solution2/syn/verilog/mm_ap_fadd_5_full_dsp_32_ip.tcl"
source "/local-scratch/localhome/msa325/ensc894/lab3/Optimized_4096/mm.prj/solution2/syn/verilog/mm_ap_fmul_2_max_dsp_32_ip.tcl"
