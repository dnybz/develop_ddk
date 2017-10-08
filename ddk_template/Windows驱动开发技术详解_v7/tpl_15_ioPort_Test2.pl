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


$tpl_input_dir="tpl_15_ioPort_Test2";
$tpl_output_dir="output/tpl_15_ioPort_Test2"."_".$Name;

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


$template->process($tpl_input_dir."/main.cpp.tpl", $vars, "$tpl_output_dir//main.cpp") || die $template->error();
$template->process($tpl_input_dir."/ConsoleTest.dsp.tpl", $vars, "$tpl_output_dir//ConsoleTest.dsp") || die $template->error();
$template->process($tpl_input_dir."/ConsoleTest.dsw.tpl", $vars, "$tpl_output_dir//ConsoleTest.dsw") || die $template->error();
$template->process($tpl_input_dir."/WinIoLib/WinIo.lib.tpl", $vars, "$tpl_output_dir//WinIoLib/WinIo.lib") || die $template->error();
$template->process($tpl_input_dir."/WinIoLib/WINIO.VXD.tpl", $vars, "$tpl_output_dir//WinIoLib/WINIO.VXD") || die $template->error();
$template->process($tpl_input_dir."/WinIoLib/WinIo.sys.tpl", $vars, "$tpl_output_dir//WinIoLib/WinIo.sys") || die $template->error();
$template->process($tpl_input_dir."/WinIoLib/WinIo.h.tpl", $vars, "$tpl_output_dir//WinIoLib/WinIo.h") || die $template->error();
$template->process($tpl_input_dir."/WinIoLib/WinIo.dll.tpl", $vars, "$tpl_output_dir//WinIoLib/WinIo.dll") || die $template->error();
$template->process($tpl_input_dir."/Debug/WinIo.sys.tpl", $vars, "$tpl_output_dir//Debug/WinIo.sys") || die $template->error();
$template->process($tpl_input_dir."/Debug/WinIo.dll.tpl", $vars, "$tpl_output_dir//Debug/WinIo.dll") || die $template->error();
