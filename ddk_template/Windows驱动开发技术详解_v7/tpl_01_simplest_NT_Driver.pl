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


$tpl_input_dir="tpl_01_simplest_NT_Driver";
$tpl_output_dir="output/tpl_01_simplest_NT_Driver"."_".$Name;

if ( ! -d   $tpl_output_dir)
{
	mkdir($tpl_output_dir);
}

########################################################################
#生成 App
my $vars = {
Name => $Name,
members=> @array,
};
#$template->process($tpl_input_dir."/MyApp.tpl", $vars, "src_gen_frame/MyApp.cpp") || die $template->error();


$template->process($tpl_input_dir."/1/makefile.tpl", $vars, "$tpl_output_dir//1/makefile") || die $template->error();
$template->process($tpl_input_dir."/1/buildfre.log.tpl", $vars, "$tpl_output_dir//1/buildfre.log") || die $template->error();
$template->process($tpl_input_dir."/1/Driver.h.tpl", $vars, "$tpl_output_dir//1/Driver.h") || die $template->error();
$template->process($tpl_input_dir."/1/Driver.cpp.tpl", $vars, "$tpl_output_dir//1/Driver.cpp") || die $template->error();
$template->process($tpl_input_dir."/1/说明.txt.tpl", $vars, "$tpl_output_dir//1/说明.txt") || die $template->error();
$template->process($tpl_input_dir."/1/Sources.tpl", $vars, "$tpl_output_dir//1/Sources") || die $template->error();
$template->process($tpl_input_dir."/2/DriverDev.dsw.tpl", $vars, "$tpl_output_dir//2/DriverDev.dsw") || die $template->error();
$template->process($tpl_input_dir."/2/sources.tpl", $vars, "$tpl_output_dir//2/sources") || die $template->error();
$template->process($tpl_input_dir."/2/DriverDev.dsp.tpl", $vars, "$tpl_output_dir//2/DriverDev.dsp") || die $template->error();
$template->process($tpl_input_dir."/2/Driver.h.tpl", $vars, "$tpl_output_dir//2/Driver.h") || die $template->error();
$template->process($tpl_input_dir."/2/Driver.cpp.tpl", $vars, "$tpl_output_dir//2/Driver.cpp") || die $template->error();
$template->process($tpl_input_dir."/2/说明.txt.tpl", $vars, "$tpl_output_dir//2/说明.txt") || die $template->error();
$template->process($tpl_input_dir."/2/MAKEFILE.tpl", $vars, "$tpl_output_dir//2/MAKEFILE") || die $template->error();
