## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e674 linker.cmd \
  package/cfg/Dsp_pe674.oe674 \

linker.cmd: package/cfg/Dsp_pe674.xdl
	$(SED) 's"^\"\(package/cfg/Dsp_pe674cfg.cmd\)\"$""\"D:/work_space_1013_light/kdsppulse/.config/xconfig_Dsp/\1\""' package/cfg/Dsp_pe674.xdl > $@
