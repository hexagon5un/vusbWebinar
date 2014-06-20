#! /usr/bin/env python

## Makes slides from simple text markup, like ReST, but for presentations.

## Basically, works as a template engine off blank_slides.tex
## Keywords: LECTURETITLE, CONTENT, DOCUMENTCLASS_SECTION, FRONTPAGE_TITLE

## Deviations from ReST: 
## Title: double "@@@@@@@@"
## Subtitle: double "+++++" 
## Sections: "+++++++++"
## Subsections: "_________"
## Frames: "========="
## Blocks: "-------"
## Lists: "*"
## Images: "%%"

templateName = "utils/blank_slides.tex"   

DOCUMENTCLASS_SLIDES = '\documentclass{beamer}\n'
DOCUMENTCLASS_HANDOUTS = '''
\documentclass[handout]{beamer}
\usepackage{pgfpages}
\pgfpagesuselayout{2 on 1} [letterpaper, border shrink=10mm]
'''

import sys

class UsageError(Exception):
    pass

if len(sys.argv) not in (2,3):
    raise UsageError, "Need one or two args: \n text-file name [(\"slides, handouts\")]" 

if len(sys.argv) == 2:
    slideName = sys.argv[1]
    documentClass = DOCUMENTCLASS_SLIDES
    
if len(sys.argv) == 3:
    slideName = sys.argv[1]
    if sys.argv[2].startswith("hand"):
        documentClass = DOCUMENTCLASS_HANDOUTS
    else:
        documentClass = DOCUMENTCLASS_SLIDES


slides = open(slideName).readlines()
template =  open(templateName).read()

lectureTitle = ""
content = ""

inFrame = False
inBlock = False
inList = False

def endList():
    global inList, content
    if inList:
        content += "    \\end{itemize}\n"  
        inList = False
        return(True)
    return(False)
   
def endBlock():
    global inBlock, content
    if inBlock:
        content += "  \\end{block}\n"       
        inBlock = False
        return(True)
    return(False)

def endFrame():
    global inFrame, content
    if inFrame:
        content += "\\end{frame}\n"
        inFrame = False
        return(True)
    return(False)
 
def endAll():
    endList()
    endBlock()
    endFrame()


for i, line in enumerate(slides):
#    line = line.strip() # controversial decision to make whitespace not count 


    if line.startswith("@@@@") and i > 1: 
        if slides[i-2].startswith("@@@@"): # title
            title = slides[i-1].strip()
        else:                    # section title
            endAll()
            content += '\n\\section{%s}\n' % slides[i-1].strip()
        continue

    if line.startswith("++++") and i > 1: 
        if slides[i-2].startswith("++++"): # lecture title
            subtitle = slides[i-1].strip()
        else:                    # section title
            endAll()
            content += '\n\\section{%s}\n' % slides[i-1].strip()
        continue

    if line.startswith("____"): # subsection title
        endAll()
        content += '\n\\subsection{%s}\n' % slides[i-1].strip()
        continue

    if line.startswith("===="): #  frame title
        endAll()
        content += '\n\\begin{frame}\n  \\frametitle{%s}\n' % \
            slides[i-1].strip()
        inFrame = True     
        continue

    if line.startswith("----"): # block title
        endList()
        if endBlock():          # test if just ending a block
            content += "      \\pause\n"
        inBlock = True                    
        content+= '  \\begin{block}{%s}\n' % slides[i-1].strip()


    if line.startswith("*"): # list element
        if not inList:
            content += "  \\begin{itemize}[<+->]\n"  
            inList = True
        content += '      \\item %s\n' % line.strip()[1:].strip() # drop the "*"            
    if line.startswith("::"): # raw latex -- pass through to beamer
        content += line.strip()[2:].strip() + "\n"

    if line.startswith(" ") or line.startswith("\t"): 
        # indentation, probably a continuation, pass
        content += "    " + line.strip() + "\n"


    if line.startswith("%!"): # vertical image name
        content += '  \\includegraphics[height=3in]{%s}\n' % \
            line.strip()[2:].strip()
    if line.startswith("%%"): # image name
        content += '  \\includegraphics[width=4in]{%s}\n' % \
            line.strip()[2:].strip()
    
endAll()

texSlides = template.replace("SUBTITLE", subtitle)
texSlides = texSlides.replace("CONTENT", content)
texSlides = texSlides.replace("DOCUMENTCLASS", documentClass)
texSlides = texSlides.replace("TITLE", title)

slideOutputName, extension = slideName.split(".", 2)
slideOutputName += ".tex"
slideOutput = file(slideOutputName, "w")
slideOutput.write(texSlides)
slideOutput.close()
