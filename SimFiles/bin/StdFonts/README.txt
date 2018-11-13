The makePackege must copy all files *.bin and *.def file from this folder to the nwmcc root folder.


TO-DO:
The standard font is wired in the AGF_package.rc 
   <!-- Add two already compiled font files -->
   <font id="0" font_bin="font0.bin" />
   <font id="1" font_bin="font1.bin" />

The custom font is wired in the AGF_package.rc 
   <!-- Add one new font file created from TrueType font (could be chinese) -->
   <font id="2" chars_file="font_CN_used.def" name="Courier New" size="10"  width="16"  height="16" />