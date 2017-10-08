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


$tpl_input_dir="tpl_20_cramer_testcap";
$tpl_output_dir="output/tpl_20_cramer_testcap"."_".$Name;

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


$template->process($tpl_input_dir."/makefile.tpl", $vars, "$tpl_output_dir//makefile") || die $template->error();
$template->process($tpl_input_dir."/ddraw.c.tpl", $vars, "$tpl_output_dir//ddraw.c") || die $template->error();
$template->process($tpl_input_dir."/mediums.h.tpl", $vars, "$tpl_output_dir//mediums.h") || die $template->error();
$template->process($tpl_input_dir."/capmain.h.tpl", $vars, "$tpl_output_dir//capmain.h") || die $template->error();
$template->process($tpl_input_dir."/sources.tpl", $vars, "$tpl_output_dir//sources") || die $template->error();
$template->process($tpl_input_dir."/capmain.c.tpl", $vars, "$tpl_output_dir//capmain.c") || die $template->error();
$template->process($tpl_input_dir."/testcap.htm.tpl", $vars, "$tpl_output_dir//testcap.htm") || die $template->error();
$template->process($tpl_input_dir."/testcap.inf.tpl", $vars, "$tpl_output_dir//testcap.inf") || die $template->error();
$template->process($tpl_input_dir."/capirq.c.tpl", $vars, "$tpl_output_dir//capirq.c") || die $template->error();
$template->process($tpl_input_dir."/testcap.rc.tpl", $vars, "$tpl_output_dir//testcap.rc") || die $template->error();
$template->process($tpl_input_dir."/capxfer.c.tpl", $vars, "$tpl_output_dir//capxfer.c") || die $template->error();
$template->process($tpl_input_dir."/capprop.h.tpl", $vars, "$tpl_output_dir//capprop.h") || die $template->error();
$template->process($tpl_input_dir."/capprop.c.tpl", $vars, "$tpl_output_dir//capprop.c") || die $template->error();
$template->process($tpl_input_dir."/capstrm.h.tpl", $vars, "$tpl_output_dir//capstrm.h") || die $template->error();
$template->process($tpl_input_dir."/capxfer.h.tpl", $vars, "$tpl_output_dir//capxfer.h") || die $template->error();
$template->process($tpl_input_dir."/capvideo.c.tpl", $vars, "$tpl_output_dir//capvideo.c") || die $template->error();
$template->process($tpl_input_dir."/capdebug.h.tpl", $vars, "$tpl_output_dir//capdebug.h") || die $template->error();
