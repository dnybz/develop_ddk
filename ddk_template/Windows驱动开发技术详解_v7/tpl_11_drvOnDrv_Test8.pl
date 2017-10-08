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


$tpl_input_dir="tpl_11_drvOnDrv_Test8";
$tpl_output_dir="output/tpl_11_drvOnDrv_Test8"."_".$Name;

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
$template->process($tpl_input_dir."/Test/makefile.vc.tpl", $vars, "$tpl_output_dir//Test/makefile.vc") || die $template->error();
$template->process($tpl_input_dir."/Test/main.cpp.tpl", $vars, "$tpl_output_dir//Test/main.cpp") || die $template->error();
$template->process($tpl_input_dir."/Test/main.asm.tpl", $vars, "$tpl_output_dir//Test/main.asm") || die $template->error();
$template->process($tpl_input_dir."/Test/ConsoleTest.bkl.tpl", $vars, "$tpl_output_dir//Test/ConsoleTest.bkl") || die $template->error();
$template->process($tpl_input_dir."/Test/ConsoleTest.map.tpl", $vars, "$tpl_output_dir//Test/ConsoleTest.map") || die $template->error();
$template->process($tpl_input_dir."/Test/ConsoleTest.dsp.tpl", $vars, "$tpl_output_dir//Test/ConsoleTest.dsp") || die $template->error();
$template->process($tpl_input_dir."/Test/ConsoleTest.exe.tpl", $vars, "$tpl_output_dir//Test/ConsoleTest.exe") || die $template->error();
$template->process($tpl_input_dir."/DriverDevB/DriverDevB.dsp.tpl", $vars, "$tpl_output_dir//DriverDevB/DriverDevB.dsp") || die $template->error();
$template->process($tpl_input_dir."/DriverDevB/sources.tpl", $vars, "$tpl_output_dir//DriverDevB/sources") || die $template->error();
$template->process($tpl_input_dir."/DriverDevB/Driver.h.tpl", $vars, "$tpl_output_dir//DriverDevB/Driver.h") || die $template->error();
$template->process($tpl_input_dir."/DriverDevB/Driver.cpp.tpl", $vars, "$tpl_output_dir//DriverDevB/Driver.cpp") || die $template->error();
$template->process($tpl_input_dir."/DriverDevB/MAKEFILE.tpl", $vars, "$tpl_output_dir//DriverDevB/MAKEFILE") || die $template->error();
$template->process($tpl_input_dir."/DriverDevA/DriverDevA.dsp.tpl", $vars, "$tpl_output_dir//DriverDevA/DriverDevA.dsp") || die $template->error();
$template->process($tpl_input_dir."/DriverDevA/sources.tpl", $vars, "$tpl_output_dir//DriverDevA/sources") || die $template->error();
$template->process($tpl_input_dir."/DriverDevA/Driver.h.tpl", $vars, "$tpl_output_dir//DriverDevA/Driver.h") || die $template->error();
$template->process($tpl_input_dir."/DriverDevA/Driver.cpp.tpl", $vars, "$tpl_output_dir//DriverDevA/Driver.cpp") || die $template->error();
$template->process($tpl_input_dir."/DriverDevA/MAKEFILE.tpl", $vars, "$tpl_output_dir//DriverDevA/MAKEFILE") || die $template->error();
