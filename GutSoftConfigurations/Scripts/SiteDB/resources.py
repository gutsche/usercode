#!/usr/bin/env python

import sys,getopt,urllib2,json,os,datetime,subprocess,shlex
from xml.dom import minidom

def getText(nodelist):
    rc = []
    for node in nodelist:
        if node.nodeType == node.TEXT_NODE:
            rc.append(node.data)
    return ''.join(rc)
    
def SiteMapping():
    CMSToWLCG = {}
    WLCGToCMS = {}
    url='https://cmsweb.cern.ch/sitedb/reports/showXMLReport?reportid=cms_to_sam.ini'
    xml = urllib2.urlopen(url).read()
    xmldoc = minidom.parseString(xml)
    items = xmldoc.getElementsByTagName("item")
    for item in items:
        cms_name = getText(item.getElementsByTagName("cms_name")[0].childNodes)
        sam_name = getText(item.getElementsByTagName("sam_name")[0].childNodes)
        if cms_name not in CMSToWLCG.keys(): CMSToWLCG[cms_name] = []
        CMSToWLCG[cms_name].append(sam_name)
        if sam_name not in WLCGToCMS.keys(): WLCGToCMS[sam_name] = []
        WLCGToCMS[sam_name].append(cms_name)
    return CMSToWLCG,WLCGToCMS
        
CMSToWLCG,WLCGToCMS = SiteMapping()
print CMSToWLCG