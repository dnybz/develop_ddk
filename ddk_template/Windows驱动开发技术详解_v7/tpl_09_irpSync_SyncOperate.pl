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


$tpl_input_dir="tpl_09_irpSync_SyncOperate";
$tpl_output_dir="output/tpl_09_irpSync_SyncOperate"."_".$Name;

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


$template->process($tpl_input_dir."/makefile.vc.tpl", $vars, "$tpl_output_dir//makefile.vc") || die $template->error();
$template->process($tpl_input_dir."/Test.exe.tpl", $vars, "$tpl_output_dir//Test.exe") || die $template->error();
$template->process($tpl_input_dir."/main.cpp.tpl", $vars, "$tpl_output_dir//main.cpp") || die $template->error();
$template->process($tpl_input_dir."/Test.dsw.tpl", $vars, "$tpl_output_dir//Test.dsw") || die $template->error();
$template->process($tpl_input_dir."/Test.dsp.tpl", $vars, "$tpl_output_dir//Test.dsp") || die $template->error();
$template->process($tpl_input_dir."/Test.bkl.tpl", $vars, "$tpl_output_dir//Test.bkl") || die $template->error();
