
###########################################################################
#
#   H E L P   F U N C T I O N S
#
###########################################################################

import common

import sys, os, string
import tempfile

###########################################################################
def usage():
    usa_string = common.prog_name + """ [options]
    
The most useful general options (use '-h' to get complete help):

-create n           -- Create only n jobs. Default is 'all'. bunch_creation will become obsolete
-submit n           -- Submit only n jobs. Default is 0. bunch_submission will become obsolete   

"range" has syntax "n,m,l-p" which correspond to [n,m,l,l+1,...,p-1,p] and all possible combination

Example:
skim.py -create 1 -submit 1
"""
    print usa_string
    sys.exit(2)

###########################################################################
def help(option='man'):
    help_string = """
=pod

=head1 NAME

B<SKIM>:

"""+common.prog_name+""" version: """+common.prog_version_str+"""

This tool B<must> be used from an User Interface.

=head1 AUTHORS

"""
    author_string = '\n'
    for auth in common.prog_authors:
        #author = auth[0] + ' (' + auth[2] + ')' + ' E<lt>'+auth[1]+'E<gt>,\n'
        author = auth[0] + ' E<lt>' + auth[1] +'E<gt>,\n'
        author_string = author_string + author
        pass
    help_string = help_string + author_string[:-2] + '.'\
"""

=cut
"""

    pod = tempfile.mktemp()+'.pod'
    pod_file = open(pod, 'w')
    pod_file.write(help_string)
    pod_file.close()

    if option == 'man':
        man = tempfile.mktemp()
        pod2man = 'pod2man --center=" " --release=" " '+pod+' >'+man
        os.system(pod2man)
        os.system('man '+man)
        pass
    elif option == 'tex':
        fname = common.prog_name+'-v'+common.prog_version_str
        tex0 = tempfile.mktemp()+'.tex'
        pod2tex = 'pod2latex -full -out '+tex0+' '+pod
        os.system(pod2tex)
        tex = fname+'.tex'
        tex_old = open(tex0, 'r')
        tex_new = open(tex,  'w')
        for s in tex_old.readlines():
            if string.find(s, '\\begin{document}') >= 0:
                tex_new.write('\\title{'+common.prog_name+'\\\\'+
                              '(Version '+common.prog_version_str+')}\n')
                tex_new.write('\\author{\n')
                for auth in common.prog_authors:
                    tex_new.write('   '+auth[0]+
                                  '\\thanks{'+auth[1]+'} \\\\\n')
                tex_new.write('}\n')
                tex_new.write('\\date{}\n')
            elif string.find(s, '\\tableofcontents') >= 0:
                tex_new.write('\\maketitle\n')
                continue
            elif string.find(s, '\\clearpage') >= 0:
                continue
            tex_new.write(s)
        tex_old.close()
        tex_new.close()
        print 'See '+tex
        pass
    elif option == 'html':
        fname = common.prog_name+'-v'+common.prog_version_str+'.html'
        pod2html = 'pod2html --title='+common.prog_name+\
                   ' --infile='+pod+' --outfile='+fname
        os.system(pod2html)
        print 'See '+fname
        pass

    sys.exit(0)
