#�ݹ��ȡ��Ŀ¼�����е�tpl �ļ�.

use Cwd;
my $g_pwd=getcwd();

my @tmp=find_and_get_tpl_filelist($g_pwd."/tpl_01_simplest_NT_Driver/");
print join("\n", @tmp);

print "###########\n";
#my @tmp2=find_and_get_tpl_filelist($g_pwd."/tpl_01_simplest_WDM_Driver/");
my @tmp=find_and_get_tpl_filelist($g_pwd."/tpl_01_simplest_NT_Driver/");
print join("\n", @tmp);
print "###########\n";

sub  find_and_get_tpl_filelist($)
{

	(my $dir)=@_;
	#logger("[find_and_get_tpl_filelis]: $dir Ŀ¼�»�ȡ���е� tpl�ļ�\n");
	#our @tpl_files=();
	our  @tpl_files=();

	use File::Find ();

	use vars qw/*name *dir *prune/;
	*name   = *File::Find::name;
	*dir    = *File::Find::dir;
	*prune  = *File::Find::prune;

	sub wanted_exe;
	sub wanted_exe 
	{
		my ($dev,$ino,$mode,$nlink,$uid,$gid);

		(($dev,$ino,$mode,$nlink,$uid,$gid) = lstat($_));
		#$name=~s/\//\\\\/g;
		if($name=~/tpl$/i && -f $name )
		#if($name=~/tpl$/i  )
		{
			$name=~s/^$dir//g; #ȥ������·��.
			#print("$name\n");
			push(@tpl_files , $name);
		}
	}
	
	File::Find::find({wanted => \&wanted_exe}, $dir);

	#logger("$dir Ŀ¼�� һ����ȡ��ȡ ".scalar(@tpl_files)."��tpl�ļ�\n");
	print ("$dir Ŀ¼�� һ����ȡ��ȡ ".scalar(@tpl_files)."��tpl�ļ�\n");
	return @tpl_files;
}

