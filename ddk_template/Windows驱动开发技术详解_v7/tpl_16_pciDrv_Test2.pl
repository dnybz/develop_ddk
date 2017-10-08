#!/usr/bin/perl
use Template;

my $config = {
INTERPOLATE  => 1,		      # expand "$var" in plain text
POST_CHOMP	  => 1,		      # cleanup whitespace
EVAL_PERL	  => 1,		      # evaluate Perl code blocks
};

my $template = Template->new();
$Name=shift  or die("Usage: $0 Name\n");
#$Name="Netware";


$tpl_input_dir="tpl_16_pciDrv_Test2";
$tpl_output_dir="output/tpl_16_pciDrv_Test2"."_".$Name;

if ( ! -d   $tpl_output_dir)
{
	mkdir($tpl_output_dir);
}

########################################################################
#Éú³É App
my $vars = {
Name => $Name,
members=> @array,
};
#$template->process($tpl_input_dir."/MyApp.tpl", $vars, "src_gen_frame/MyApp.cpp") || die $template->error();


$template->process($tpl_input_dir."/NT_Driver.dsw.tpl", $vars, "$tpl_output_dir//NT_Driver.dsw") || die $template->error();
$template->process($tpl_input_dir."/NT_Driver/NT_Driver/Ioctls.h.tpl", $vars, "$tpl_output_dir//NT_Driver/NT_Driver/Ioctls.h") || die $template->error();
$template->process($tpl_input_dir."/NT_Driver/NT_Driver/DriverDev.dsp.tpl", $vars, "$tpl_output_dir//NT_Driver/NT_Driver/DriverDev.dsp") || die $template->error();
$template->process($tpl_input_dir."/NT_Driver/NT_Driver/Driver.h.tpl", $vars, "$tpl_output_dir//NT_Driver/NT_Driver/Driver.h") || die $template->error();
$template->process($tpl_input_dir."/NT_Driver/NT_Driver/Driver.cpp.tpl", $vars, "$tpl_output_dir//NT_Driver/NT_Driver/Driver.cpp") || die $template->error();
$template->process($tpl_input_dir."/Test/main.cpp.tpl", $vars, "$tpl_output_dir//Test/main.cpp") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI001F2.bin.tpl", $vars, "$tpl_output_dir//Test/PCI001F2.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI001F3.bin.tpl", $vars, "$tpl_output_dir//Test/PCI001F3.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI00020.bin.tpl", $vars, "$tpl_output_dir//Test/PCI00020.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI001E0.bin.tpl", $vars, "$tpl_output_dir//Test/PCI001E0.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI001F1.bin.tpl", $vars, "$tpl_output_dir//Test/PCI001F1.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI001C0.bin.tpl", $vars, "$tpl_output_dir//Test/PCI001C0.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/function.h.tpl", $vars, "$tpl_output_dir//Test/function.h") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI001B0.bin.tpl", $vars, "$tpl_output_dir//Test/PCI001B0.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI001D0.bin.tpl", $vars, "$tpl_output_dir//Test/PCI001D0.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI00000.bin.tpl", $vars, "$tpl_output_dir//Test/PCI00000.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI001D1.bin.tpl", $vars, "$tpl_output_dir//Test/PCI001D1.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI001D3.bin.tpl", $vars, "$tpl_output_dir//Test/PCI001D3.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/Test.dsp.tpl", $vars, "$tpl_output_dir//Test/Test.dsp") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI02000.bin.tpl", $vars, "$tpl_output_dir//Test/PCI02000.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI001D7.bin.tpl", $vars, "$tpl_output_dir//Test/PCI001D7.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI001F0.bin.tpl", $vars, "$tpl_output_dir//Test/PCI001F0.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/function.cpp.tpl", $vars, "$tpl_output_dir//Test/function.cpp") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI00021.bin.tpl", $vars, "$tpl_output_dir//Test/PCI00021.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI00010.bin.tpl", $vars, "$tpl_output_dir//Test/PCI00010.bin") || die $template->error();
$template->process($tpl_input_dir."/Test/PCI001D2.bin.tpl", $vars, "$tpl_output_dir//Test/PCI001D2.bin") || die $template->error();
