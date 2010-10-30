#include "stdafx.h"

#include "license_dialog.h"
#include "defines.h"

#include ".\license_dialog.h"

#include <iostream>
#include <sstream>
using std::ostringstream;
using std::endl;



CLicenseDlg::CLicenseDlg() : CDialog(CLicenseDlg::IDD)
{
}

void CLicenseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BOOL CLicenseDlg::OnInitDialog()
{
	if(!CDialog::OnInitDialog())
		return FALSE;


	ostringstream out;

out << "		    GNU GENERAL PUBLIC LICENSE" << "\r\n";
out << "		       Version 2, June 1991" << "\r\n";
out << "" << "\r\n";
out << " Copyright (C) 1989, 1991 Free Software Foundation, Inc." << "\r\n";
out << "                       59 Temple Place, Suite 330, Boston, MA  02111-1307  USA" << "\r\n";
out << " Everyone is permitted to copy and distribute verbatim copies" << "\r\n";
out << " of this license document, but changing it is not allowed." << "\r\n";
out << "" << "\r\n";
out << "			    Preamble" << "\r\n";
out << "" << "\r\n";
out << "  The licenses for most software are designed to take away your" << "\r\n";
out << "freedom to share and change it.  By contrast, the GNU General Public" << "\r\n";
out << "License is intended to guarantee your freedom to share and change free" << "\r\n";
out << "software--to make sure the software is free for all its users.  This" << "\r\n";
out << "General Public License applies to most of the Free Software" << "\r\n";
out << "Foundation's software and to any other program whose authors commit to" << "\r\n";
out << "using it.  (Some other Free Software Foundation software is covered by" << "\r\n";
out << "the GNU Library General Public License instead.)  You can apply it to" << "\r\n";
out << "your programs, too." << "\r\n";
out << "" << "\r\n";
out << "  When we speak of free software, we are referring to freedom, not" << "\r\n";
out << "price.  Our General Public Licenses are designed to make sure that you" << "\r\n";
out << "have the freedom to distribute copies of free software (and charge for" << "\r\n";
out << "this service if you wish), that you receive source code or can get it" << "\r\n";
out << "if you want it, that you can change the software or use pieces of it" << "\r\n";
out << "in new free programs; and that you know you can do these things." << "\r\n";
out << "" << "\r\n";
out << "  To protect your rights, we need to make restrictions that forbid" << "\r\n";
out << "anyone to deny you these rights or to ask you to surrender the rights." << "\r\n";
out << "These restrictions translate to certain responsibilities for you if you" << "\r\n";
out << "distribute copies of the software, or if you modify it." << "\r\n";
out << "" << "\r\n";
out << "  For example, if you distribute copies of such a program, whether" << "\r\n";
out << "gratis or for a fee, you must give the recipients all the rights that" << "\r\n";
out << "you have.  You must make sure that they, too, receive or can get the" << "\r\n";
out << "source code.  And you must show them these terms so they know their" << "\r\n";
out << "rights." << "\r\n";
out << "" << "\r\n";
out << "  We protect your rights with two steps: (1) copyright the software, and" << "\r\n";
out << "(2) offer you this license which gives you legal permission to copy," << "\r\n";
out << "distribute and/or modify the software." << "\r\n";
out << "" << "\r\n";
out << "  Also, for each author's protection and ours, we want to make certain" << "\r\n";
out << "that everyone understands that there is no warranty for this free" << "\r\n";
out << "software.  If the software is modified by someone else and passed on, we" << "\r\n";
out << "want its recipients to know that what they have is not the original, so" << "\r\n";
out << "that any problems introduced by others will not reflect on the original" << "\r\n";
out << "authors' reputations." << "\r\n";
out << "" << "\r\n";
out << "  Finally, any free program is threatened constantly by software" << "\r\n";
out << "patents.  We wish to avoid the danger that redistributors of a free" << "\r\n";
out << "program will individually obtain patent licenses, in effect making the" << "\r\n";
out << "program proprietary.  To prevent this, we have made it clear that any" << "\r\n";
out << "patent must be licensed for everyone's free use or not licensed at all." << "\r\n";
out << "" << "\r\n";
out << "  The precise terms and conditions for copying, distribution and" << "\r\n";
out << "modification follow." << "\r\n";
out << "" << "\r\n";
out << "		    GNU GENERAL PUBLIC LICENSE" << "\r\n";
out << "   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION" << "\r\n";
out << "" << "\r\n";
out << "  0. This License applies to any program or other work which contains" << "\r\n";
out << "a notice placed by the copyright holder saying it may be distributed" << "\r\n";
out << "under the terms of this General Public License.  The \"Program\", below," << "\r\n";
out << "refers to any such program or work, and a \"work based on the Program\"" << "\r\n";
out << "means either the Program or any derivative work under copyright law:" << "\r\n";
out << "that is to say, a work containing the Program or a portion of it," << "\r\n";
out << "either verbatim or with modifications and/or translated into another" << "\r\n";
out << "language.  (Hereinafter, translation is included without limitation in" << "\r\n";
out << "the term \"modification\".)  Each licensee is addressed as \"you\"." << "\r\n";
out << "" << "\r\n";
out << "Activities other than copying, distribution and modification are not" << "\r\n";
out << "covered by this License; they are outside its scope.  The act of" << "\r\n";
out << "running the Program is not restricted, and the output from the Program" << "\r\n";
out << "is covered only if its contents constitute a work based on the" << "\r\n";
out << "Program (independent of having been made by running the Program)." << "\r\n";
out << "Whether that is true depends on what the Program does." << "\r\n";
out << "" << "\r\n";
out << "  1. You may copy and distribute verbatim copies of the Program's" << "\r\n";
out << "source code as you receive it, in any medium, provided that you" << "\r\n";
out << "conspicuously and appropriately publish on each copy an appropriate" << "\r\n";
out << "copyright notice and disclaimer of warranty; keep intact all the" << "\r\n";
out << "notices that refer to this License and to the absence of any warranty;" << "\r\n";
out << "and give any other recipients of the Program a copy of this License" << "\r\n";
out << "along with the Program." << "\r\n";
out << "" << "\r\n";
out << "You may charge a fee for the physical act of transferring a copy, and" << "\r\n";
out << "you may at your option offer warranty protection in exchange for a fee." << "\r\n";
out << "" << "\r\n";
out << "  2. You may modify your copy or copies of the Program or any portion" << "\r\n";
out << "of it, thus forming a work based on the Program, and copy and" << "\r\n";
out << "distribute such modifications or work under the terms of Section 1" << "\r\n";
out << "above, provided that you also meet all of these conditions:" << "\r\n";
out << "" << "\r\n";
out << "    a) You must cause the modified files to carry prominent notices" << "\r\n";
out << "    stating that you changed the files and the date of any change." << "\r\n";
out << "" << "\r\n";
out << "    b) You must cause any work that you distribute or publish, that in" << "\r\n";
out << "    whole or in part contains or is derived from the Program or any" << "\r\n";
out << "    part thereof, to be licensed as a whole at no charge to all third" << "\r\n";
out << "    parties under the terms of this License." << "\r\n";
out << "" << "\r\n";
out << "    c) If the modified program normally reads commands interactively" << "\r\n";
out << "    when run, you must cause it, when started running for such" << "\r\n";
out << "    interactive use in the most ordinary way, to print or display an" << "\r\n";
out << "    announcement including an appropriate copyright notice and a" << "\r\n";
out << "    notice that there is no warranty (or else, saying that you provide" << "\r\n";
out << "    a warranty) and that users may redistribute the program under" << "\r\n";
out << "    these conditions, and telling the user how to view a copy of this" << "\r\n";
out << "    License.  (Exception: if the Program itself is interactive but" << "\r\n";
out << "    does not normally print such an announcement, your work based on" << "\r\n";
out << "    the Program is not required to print an announcement.)" << "\r\n";
out << "" << "\r\n";
out << "These requirements apply to the modified work as a whole.  If" << "\r\n";
out << "identifiable sections of that work are not derived from the Program," << "\r\n";
out << "and can be reasonably considered independent and separate works in" << "\r\n";
out << "themselves, then this License, and its terms, do not apply to those" << "\r\n";
out << "sections when you distribute them as separate works.  But when you" << "\r\n";
out << "distribute the same sections as part of a whole which is a work based" << "\r\n";
out << "on the Program, the distribution of the whole must be on the terms of" << "\r\n";
out << "this License, whose permissions for other licensees extend to the" << "\r\n";
out << "entire whole, and thus to each and every part regardless of who wrote it." << "\r\n";
out << "" << "\r\n";
out << "Thus, it is not the intent of this section to claim rights or contest" << "\r\n";
out << "your rights to work written entirely by you; rather, the intent is to" << "\r\n";
out << "exercise the right to control the distribution of derivative or" << "\r\n";
out << "collective works based on the Program." << "\r\n";
out << "" << "\r\n";
out << "In addition, mere aggregation of another work not based on the Program" << "\r\n";
out << "with the Program (or with a work based on the Program) on a volume of" << "\r\n";
out << "a storage or distribution medium does not bring the other work under" << "\r\n";
out << "the scope of this License." << "\r\n";
out << "" << "\r\n";
out << "  3. You may copy and distribute the Program (or a work based on it," << "\r\n";
out << "under Section 2) in object code or executable form under the terms of" << "\r\n";
out << "Sections 1 and 2 above provided that you also do one of the following:" << "\r\n";
out << "" << "\r\n";
out << "    a) Accompany it with the complete corresponding machine-readable" << "\r\n";
out << "    source code, which must be distributed under the terms of Sections" << "\r\n";
out << "    1 and 2 above on a medium customarily used for software interchange; or," << "\r\n";
out << "" << "\r\n";
out << "    b) Accompany it with a written offer, valid for at least three" << "\r\n";
out << "    years, to give any third party, for a charge no more than your" << "\r\n";
out << "    cost of physically performing source distribution, a complete" << "\r\n";
out << "    machine-readable copy of the corresponding source code, to be" << "\r\n";
out << "    distributed under the terms of Sections 1 and 2 above on a medium" << "\r\n";
out << "    customarily used for software interchange; or," << "\r\n";
out << "" << "\r\n";
out << "    c) Accompany it with the information you received as to the offer" << "\r\n";
out << "    to distribute corresponding source code.  (This alternative is" << "\r\n";
out << "    allowed only for noncommercial distribution and only if you" << "\r\n";
out << "    received the program in object code or executable form with such" << "\r\n";
out << "    an offer, in accord with Subsection b above.)" << "\r\n";
out << "" << "\r\n";
out << "The source code for a work means the preferred form of the work for" << "\r\n";
out << "making modifications to it.  For an executable work, complete source" << "\r\n";
out << "code means all the source code for all modules it contains, plus any" << "\r\n";
out << "associated interface definition files, plus the scripts used to" << "\r\n";
out << "control compilation and installation of the executable.  However, as a" << "\r\n";
out << "special exception, the source code distributed need not include" << "\r\n";
out << "anything that is normally distributed (in either source or binary" << "\r\n";
out << "form) with the major components (compiler, kernel, and so on) of the" << "\r\n";
out << "operating system on which the executable runs, unless that component" << "\r\n";
out << "itself accompanies the executable." << "\r\n";
out << "" << "\r\n";
out << "If distribution of executable or object code is made by offering" << "\r\n";
out << "access to copy from a designated place, then offering equivalent" << "\r\n";
out << "access to copy the source code from the same place counts as" << "\r\n";
out << "distribution of the source code, even though third parties are not" << "\r\n";
out << "compelled to copy the source along with the object code." << "\r\n";
out << "" << "\r\n";
out << "  4. You may not copy, modify, sublicense, or distribute the Program" << "\r\n";
out << "except as expressly provided under this License.  Any attempt" << "\r\n";
out << "otherwise to copy, modify, sublicense or distribute the Program is" << "\r\n";
out << "void, and will automatically terminate your rights under this License." << "\r\n";
out << "However, parties who have received copies, or rights, from you under" << "\r\n";
out << "this License will not have their licenses terminated so long as such" << "\r\n";
out << "parties remain in full compliance." << "\r\n";
out << "" << "\r\n";
out << "  5. You are not required to accept this License, since you have not" << "\r\n";
out << "signed it.  However, nothing else grants you permission to modify or" << "\r\n";
out << "distribute the Program or its derivative works.  These actions are" << "\r\n";
out << "prohibited by law if you do not accept this License.  Therefore, by" << "\r\n";
out << "modifying or distributing the Program (or any work based on the" << "\r\n";
out << "Program), you indicate your acceptance of this License to do so, and" << "\r\n";
out << "all its terms and conditions for copying, distributing or modifying" << "\r\n";
out << "the Program or works based on it." << "\r\n";
out << "" << "\r\n";
out << "  6. Each time you redistribute the Program (or any work based on the" << "\r\n";
out << "Program), the recipient automatically receives a license from the" << "\r\n";
out << "original licensor to copy, distribute or modify the Program subject to" << "\r\n";
out << "these terms and conditions.  You may not impose any further" << "\r\n";
out << "restrictions on the recipients' exercise of the rights granted herein." << "\r\n";
out << "You are not responsible for enforcing compliance by third parties to" << "\r\n";
out << "this License." << "\r\n";
out << "" << "\r\n";
out << "  7. If, as a consequence of a court judgment or allegation of patent" << "\r\n";
out << "infringement or for any other reason (not limited to patent issues)," << "\r\n";
out << "conditions are imposed on you (whether by court order, agreement or" << "\r\n";
out << "otherwise) that contradict the conditions of this License, they do not" << "\r\n";
out << "excuse you from the conditions of this License.  If you cannot" << "\r\n";
out << "distribute so as to satisfy simultaneously your obligations under this" << "\r\n";
out << "License and any other pertinent obligations, then as a consequence you" << "\r\n";
out << "may not distribute the Program at all.  For example, if a patent" << "\r\n";
out << "license would not permit royalty-free redistribution of the Program by" << "\r\n";
out << "all those who receive copies directly or indirectly through you, then" << "\r\n";
out << "the only way you could satisfy both it and this License would be to" << "\r\n";
out << "refrain entirely from distribution of the Program." << "\r\n";
out << "" << "\r\n";
out << "If any portion of this section is held invalid or unenforceable under" << "\r\n";
out << "any particular circumstance, the balance of the section is intended to" << "\r\n";
out << "apply and the section as a whole is intended to apply in other" << "\r\n";
out << "circumstances." << "\r\n";
out << "" << "\r\n";
out << "It is not the purpose of this section to induce you to infringe any" << "\r\n";
out << "patents or other property right claims or to contest validity of any" << "\r\n";
out << "such claims; this section has the sole purpose of protecting the" << "\r\n";
out << "integrity of the free software distribution system, which is" << "\r\n";
out << "implemented by public license practices.  Many people have made" << "\r\n";
out << "generous contributions to the wide range of software distributed" << "\r\n";
out << "through that system in reliance on consistent application of that" << "\r\n";
out << "system; it is up to the author/donor to decide if he or she is willing" << "\r\n";
out << "to distribute software through any other system and a licensee cannot" << "\r\n";
out << "impose that choice." << "\r\n";
out << "" << "\r\n";
out << "This section is intended to make thoroughly clear what is believed to" << "\r\n";
out << "be a consequence of the rest of this License." << "\r\n";
out << "" << "\r\n";
out << "  8. If the distribution and/or use of the Program is restricted in" << "\r\n";
out << "certain countries either by patents or by copyrighted interfaces, the" << "\r\n";
out << "original copyright holder who places the Program under this License" << "\r\n";
out << "may add an explicit geographical distribution limitation excluding" << "\r\n";
out << "those countries, so that distribution is permitted only in or among" << "\r\n";
out << "countries not thus excluded.  In such case, this License incorporates" << "\r\n";
out << "the limitation as if written in the body of this License." << "\r\n";
out << "" << "\r\n";
out << "  9. The Free Software Foundation may publish revised and/or new versions" << "\r\n";
out << "of the General Public License from time to time.  Such new versions will" << "\r\n";
out << "be similar in spirit to the present version, but may differ in detail to" << "\r\n";
out << "address new problems or concerns." << "\r\n";
out << "" << "\r\n";
out << "Each version is given a distinguishing version number.  If the Program" << "\r\n";
out << "specifies a version number of this License which applies to it and \"any" << "\r\n";
out << "later version\", you have the option of following the terms and conditions" << "\r\n";
out << "either of that version or of any later version published by the Free" << "\r\n";
out << "Software Foundation.  If the Program does not specify a version number of" << "\r\n";
out << "this License, you may choose any version ever published by the Free Software" << "\r\n";
out << "Foundation." << "\r\n";
out << "" << "\r\n";
out << "  10. If you wish to incorporate parts of the Program into other free" << "\r\n";
out << "programs whose distribution conditions are different, write to the author" << "\r\n";
out << "to ask for permission.  For software which is copyrighted by the Free" << "\r\n";
out << "Software Foundation, write to the Free Software Foundation; we sometimes" << "\r\n";
out << "make exceptions for this.  Our decision will be guided by the two goals" << "\r\n";
out << "of preserving the free status of all derivatives of our free software and" << "\r\n";
out << "of promoting the sharing and reuse of software generally." << "\r\n";
out << "" << "\r\n";
out << "			    NO WARRANTY" << "\r\n";
out << "" << "\r\n";
out << "  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY" << "\r\n";
out << "FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN" << "\r\n";
out << "OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES" << "\r\n";
out << "PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED" << "\r\n";
out << "OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF" << "\r\n";
out << "MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS" << "\r\n";
out << "TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE" << "\r\n";
out << "PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING," << "\r\n";
out << "REPAIR OR CORRECTION." << "\r\n";
out << "" << "\r\n";
out << "  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING" << "\r\n";
out << "WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR" << "\r\n";
out << "REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES," << "\r\n";
out << "INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING" << "\r\n";
out << "OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED" << "\r\n";
out << "TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY" << "\r\n";
out << "YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER" << "\r\n";
out << "PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE" << "\r\n";
out << "POSSIBILITY OF SUCH DAMAGES." << "\r\n";
out << "" << "\r\n";
out << "		     END OF TERMS AND CONDITIONS" << "\r\n";


this->SetDlgItemText(IDC_LICENSE_EDIT, out.str().c_str());




	return TRUE;
}


BEGIN_MESSAGE_MAP(CLicenseDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()



void CLicenseDlg::OnBnClickedOk()
{
	OnOK();
}


