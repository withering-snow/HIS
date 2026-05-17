"""
Markdown to Word 转换脚本
用法: python md_to_docx.py 总结报告.md 总结报告.docx
      python md_to_docx.py 答辩指南.md 答辩指南.docx
"""

import re
import sys
import os
from xml.etree import ElementTree as ET
from xml.sax.saxutils import escape as xml_escape
from zipfile import ZipFile, ZIP_DEFLATED
from datetime import datetime
import shutil
import copy

# ─── Word 文档的 XML 模板 ───────────────────────────────────────────────

WORD_NS = 'http://schemas.openxmlformats.org/wordprocessingml/2006/main'
R_NS = 'http://schemas.openxmlformats.org/officeDocument/2006/relationships'
REL_NS = 'http://schemas.openxmlformats.org/package/2006/relationships'
CONTENT_TYPE_NS = 'http://schemas.openxmlformats.org/package/2006/content-types'

def ns(tag):
    return f'{{http://schemas.openxmlformats.org/wordprocessingml/2006/main}}{tag}'

def rns(tag):
    return f'{{http://schemas.openxmlformats.org/officeDocument/2006/relationships}}{tag}'

def make_run(text, bold=False, color=None, size=None, font=None):
    """创建一个 run 元素"""
    r = ET.Element(ns('r'))
    rpr = ET.SubElement(r, ns('rPr'))
    if bold:
        ET.SubElement(rpr, ns('b'))
    if color:
        c = ET.SubElement(rpr, ns('color'))
        c.set(ns('val'), color)
    if size:
        sz = ET.SubElement(rpr, ns('sz'))
        sz.set(ns('val'), str(size))
    if font:
        rf = ET.SubElement(rpr, ns('rFonts'))
        rf.set(ns('ascii'), font)
        rf.set(ns('hAnsi'), font)
    t = ET.SubElement(r, ns('t'))
    t.set('{http://www.w3.org/XML/1998/namespace}space', 'preserve')
    t.text = text
    return r

def make_paragraph(runs=None, style=None, spacing_before=0, spacing_after=0, alignment=None):
    """创建一个段落元素"""
    p = ET.Element(ns('p'))
    ppr = ET.SubElement(p, ns('pPr'))
    
    if style:
        pstyle = ET.SubElement(ppr, ns('pStyle'))
        pstyle.set(ns('val'), style)
    
    if spacing_before or spacing_after:
        sp = ET.SubElement(ppr, ns('spacing'))
        if spacing_before:
            sp.set(ns('before'), str(spacing_before))
        if spacing_after:
            sp.set(ns('after'), str(spacing_after))
    
    if alignment:
        jc = ET.SubElement(ppr, ns('jc'))
        jc.set(ns('val'), alignment)
    
    if runs:
        for run in runs:
            p.append(run)
    return p

def make_table(headers, rows):
    """创建表格"""
    tbl = ET.Element(ns('tbl'))
    
    # 表格属性
    tblpr = ET.SubElement(tbl, ns('tblPr'))
    tblw = ET.SubElement(tblpr, ns('tblW'))
    tblw.set(ns('w'), '5000')
    tblw.set(ns('type'), 'pct')
    
    # 边框
    tblborders = ET.SubElement(tblpr, ns('tblBorders'))
    for border_name in ['top', 'left', 'bottom', 'right', 'insideH', 'insideV']:
        border = ET.SubElement(tblborders, ns(border_name))
        border.set(ns('val'), 'single')
        border.set(ns('sz'), '4')
        border.set(ns('space'), '0')
        border.set(ns('color'), '000000')
    
    # 表头
    tr_header = ET.SubElement(tbl, ns('tr'))
    trpr = ET.SubElement(tr_header, ns('trPr'))
    trheight = ET.SubElement(trpr, ns('trHeight'))
    trheight.set(ns('val'), '400')
    
    for header in headers:
        tc = ET.SubElement(tr_header, ns('tc'))
        tcpr = ET.SubElement(tc, ns('tcPr'))
        tcw = ET.SubElement(tcpr, ns('tcW'))
        tcw.set(ns('w'), str(int(5000 / len(headers))))
        tcw.set(ns('type'), 'dxa')
        shading = ET.SubElement(tcpr, ns('shd'))
        shading.set(ns('val'), 'clear')
        shading.set(ns('color'), 'auto')
        shading.set(ns('fill'), 'D9E2F3')
        
        p = ET.SubElement(tc, ns('p'))
        ppr = ET.SubElement(p, ns('pPr'))
        jc = ET.SubElement(ppr, ns('jc'))
        jc.set(ns('val'), 'center')
        p.append(make_run(header, bold=True, size=20))
    
    # 数据行
    for row in rows:
        tr = ET.SubElement(tbl, ns('tr'))
        for cell_text in row:
            tc = ET.SubElement(tr, ns('tc'))
            tcpr = ET.SubElement(tc, ns('tcPr'))
            tcw = ET.SubElement(tcpr, ns('tcW'))
            tcw.set(ns('w'), str(int(5000 / len(headers))))
            tcw.set(ns('type'), 'dxa')
            
            p = ET.SubElement(tc, ns('p'))
            p.append(make_run(str(cell_text), size=20))
    
    return tbl

# ─── Markdown 解析器（简化版） ──────────────────────────────────────────

class MarkdownParser:
    def __init__(self):
        self.blocks = []
    
    def parse(self, text):
        """解析 Markdown 文本为 block 列表"""
        lines = text.split('\n')
        i = 0
        in_code_block = False
        code_buffer = []
        in_table = False
        table_buffer = []
        
        while i < len(lines):
            line = lines[i]
            
            # 代码块
            if line.startswith('```'):
                if in_code_block:
                    self.blocks.append(('code', '\n'.join(code_buffer)))
                    code_buffer = []
                    in_code_block = False
                else:
                    in_code_block = True
                i += 1
                continue
            
            if in_code_block:
                code_buffer.append(line)
                i += 1
                continue
            
            # 空行
            if not line.strip():
                if in_table and table_buffer:
                    self._parse_table(table_buffer)
                    table_buffer = []
                    in_table = False
                i += 1
                continue
            
            # 表格
            if '|' in line and line.strip().startswith('|'):
                in_table = True
                table_buffer.append(line)
                i += 1
                continue
            else:
                if in_table and table_buffer:
                    self._parse_table(table_buffer)
                    table_buffer = []
                    in_table = False
            
            # 标题
            if line.startswith('#'):
                level = len(re.match(r'#+', line).group())
                title = line.lstrip('#').strip()
                self.blocks.append(('heading', level, title))
                i += 1
                continue
            
            # 分隔线
            if re.match(r'^[-*_]{3,}$', line.strip()):
                self.blocks.append(('hr',))
                i += 1
                continue
            
            # 列表
            if re.match(r'^\s*[-*+]\s', line):
                items = []
                while i < len(lines) and re.match(r'^\s*[-*+]\s', lines[i]):
                    items.append(re.sub(r'^\s*[-*+]\s', '', lines[i]))
                    i += 1
                self.blocks.append(('list', items))
                continue
            
            # 有序列表
            if re.match(r'^\s*\d+[.)]\s', line):
                items = []
                while i < len(lines) and re.match(r'^\s*\d+[.)]\s', lines[i]):
                    items.append(re.sub(r'^\s*\d+[.)]\s', '', lines[i]))
                    i += 1
                self.blocks.append(('ordered_list', items))
                continue
            
            # 普通段落（支持多行）
            para_lines = []
            while i < len(lines) and lines[i].strip() and not lines[i].startswith('#') and not lines[i].startswith('```') and not lines[i].startswith('|'):
                para_lines.append(lines[i])
                i += 1
            if para_lines:
                self.blocks.append(('paragraph', '\n'.join(para_lines)))
                continue
            
            i += 1
        
        # 处理末尾未闭合的表格
        if in_table and table_buffer:
            self._parse_table(table_buffer)
    
    def _parse_table(self, lines):
        """解析表格"""
        if len(lines) < 2:
            return
        
        headers = [h.strip() for h in lines[0].split('|') if h.strip()]
        rows = []
        for line in lines[2:]:  # 跳过分隔行
            cells = [c.strip() for c in line.split('|') if c.strip()]
            if cells:
                rows.append(cells)
        
        if headers:
            self.blocks.append(('table', headers, rows))
    
    def _inline_format(self, text):
        """解析行内格式，返回 run 列表"""
        runs = []
        # 处理加粗 **text**
        parts = re.split(r'(\*\*.*?\*\*)', text)
        for part in parts:
            if part.startswith('**') and part.endswith('**'):
                runs.append(('bold', part[2:-2]))
            else:
                # 处理行内代码 `code`
                subparts = re.split(r'(`.*?`)', part)
                for sp in subparts:
                    if sp.startswith('`') and sp.endswith('`'):
                        runs.append(('code', sp[1:-1]))
                    else:
                        runs.append(('text', sp))
        return runs


# ─── Word 文档生成器 ────────────────────────────────────────────────────

class DocxGenerator:
    def __init__(self):
        self.body = ET.Element(ns('body'))
        self.rels = []
        self.rel_id = 1
    
    def add_heading(self, level, text):
        style_map = {1: 'Heading1', 2: 'Heading2', 3: 'Heading3', 4: 'Heading4'}
        style = style_map.get(level, 'Heading1')
        
        runs = []
        for fmt, t in self._parse_inline(text):
            if fmt == 'bold':
                runs.append(make_run(t, bold=True, size={1: 36, 2: 28, 3: 24, 4: 22}.get(level, 24)))
            elif fmt == 'code':
                runs.append(make_run(t, font='Courier New', size={1: 36, 2: 28, 3: 24, 4: 22}.get(level, 24)))
            else:
                runs.append(make_run(t, size={1: 36, 2: 28, 3: 24, 4: 22}.get(level, 24)))
        
        spacing = {1: 240, 2: 180, 3: 120, 4: 120}.get(level, 120)
        self.body.append(make_paragraph(runs, style=style, spacing_before=spacing, spacing_after=120))
    
    def add_paragraph(self, text):
        """添加段落"""
        lines = text.split('\n')
        for line in lines:
            if not line.strip():
                continue
            runs = []
            for fmt, t in self._parse_inline(line):
                if fmt == 'bold':
                    runs.append(make_run(t, bold=True, size=22))
                elif fmt == 'code':
                    runs.append(make_run(t, font='Courier New', size=20, color='333333'))
                else:
                    runs.append(make_run(t, size=22))
            self.body.append(make_paragraph(runs, spacing_after=60))
    
    def add_code_block(self, text):
        """添加代码块"""
        for line in text.split('\n'):
            if not line.strip():
                continue
            runs = [make_run(line, font='Courier New', size=18, color='1a1a1a')]
            p = make_paragraph(runs, spacing_after=0)
            # 添加灰色背景
            ppr = p.find(ns('pPr'))
            if ppr is None:
                ppr = ET.Element(ns('pPr'))
                p.insert(0, ppr)
            shading = ET.SubElement(ppr, ns('shd'))
            shading.set(ns('val'), 'clear')
            shading.set(ns('color'), 'auto')
            shading.set(ns('fill'), 'F2F2F2')
            self.body.append(p)
    
    def add_list(self, items, ordered=False):
        """添加列表"""
        for i, item in enumerate(items):
            runs = []
            for fmt, t in self._parse_inline(item):
                if fmt == 'bold':
                    runs.append(make_run(t, bold=True, size=22))
                elif fmt == 'code':
                    runs.append(make_run(t, font='Courier New', size=20, color='333333'))
                else:
                    runs.append(make_run(t, size=22))
            
            prefix = f"{i+1}. " if ordered else "• "
            prefix_run = make_run(prefix, size=22)
            all_runs = [prefix_run] + runs
            self.body.append(make_paragraph(all_runs, spacing_after=40))
    
    def add_table(self, headers, rows):
        """添加表格"""
        self.body.append(make_table(headers, rows))
        # 表后空行
        self.body.append(make_paragraph([make_run('', size=22)], spacing_after=60))
    
    def add_hr(self):
        """添加分隔线"""
        p = ET.Element(ns('p'))
        ppr = ET.SubElement(p, ns('pPr'))
        pbdr = ET.SubElement(ppr, ns('pBdr'))
        bottom = ET.SubElement(pbdr, ns('bottom'))
        bottom.set(ns('val'), 'single')
        bottom.set(ns('sz'), '6')
        bottom.set(ns('space'), '1')
        bottom.set(ns('color'), '999999')
        self.body.append(p)
        self.body.append(make_paragraph([make_run('', size=22)], spacing_after=60))
    
    def _parse_inline(self, text):
        """解析行内格式"""
        results = []
        parts = re.split(r'(\*\*.*?\*\*|`.*?`)', text)
        for part in parts:
            if part.startswith('**') and part.endswith('**'):
                results.append(('bold', part[2:-2]))
            elif part.startswith('`') and part.endswith('`'):
                results.append(('code', part[1:-1]))
            elif part:
                results.append(('text', part))
        return results
    
    def save(self, output_path):
        """保存为 .docx 文件"""
        # 创建 document.xml
        document = ET.Element('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}document')
        document.append(self.body)
        
        # 创建 [Content_Types].xml
        content_types = ET.Element('{http://schemas.openxmlformats.org/package/2006/content-types}Types')
        default_types = [
            ('rels', 'application/vnd.openxmlformats-package.relationships+xml'),
            ('xml', 'application/xml'),
        ]
        for ext, ct in default_types:
            elem = ET.SubElement(content_types, '{http://schemas.openxmlformats.org/package/2006/content-types}Default')
            elem.set('Extension', ext)
            elem.set('ContentType', ct)
        
        override_types = [
            ('/word/document.xml', 'application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml'),
            ('/word/styles.xml', 'application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml'),
        ]
        for part, ct in override_types:
            elem = ET.SubElement(content_types, '{http://schemas.openxmlformats.org/package/2006/content-types}Override')
            elem.set('PartName', part)
            elem.set('ContentType', ct)
        
        # 创建 styles.xml
        styles = ET.Element('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}styles')
        
        # 默认样式
        doc_default = ET.SubElement(styles, '{http://schemas.openxmlformats.org/wordprocessingml/2006/main}docDefaults')
        rpr_default = ET.SubElement(ET.SubElement(doc_default, '{http://schemas.openxmlformats.org/wordprocessingml/2006/main}rPrDefault'), '{http://schemas.openxmlformats.org/wordprocessingml/2006/main}rPr')
        sz = ET.SubElement(rpr_default, '{http://schemas.openxmlformats.org/wordprocessingml/2006/main}sz')
        sz.set('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}val', '22')
        szCs = ET.SubElement(rpr_default, '{http://schemas.openxmlformats.org/wordprocessingml/2006/main}szCs')
        szCs.set('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}val', '22')
        
        # 标题样式
        for level, (name, size, bold) in enumerate([
            ('Heading1', 36, True),
            ('Heading2', 28, True),
            ('Heading3', 24, True),
            ('Heading4', 22, True),
        ], 1):
            style = ET.SubElement(styles, '{http://schemas.openxmlformats.org/wordprocessingml/2006/main}style')
            style.set('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}type', 'paragraph')
            style.set('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}styleId', name)
            
            sname = ET.SubElement(style, '{http://schemas.openxmlformats.org/wordprocessingml/2006/main}name')
            sname.set('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}val', name)
            
            srpr = ET.SubElement(style, '{http://schemas.openxmlformats.org/wordprocessingml/2006/main}rPr')
            sb = ET.SubElement(srpr, '{http://schemas.openxmlformats.org/wordprocessingml/2006/main}b')
            ssz = ET.SubElement(srpr, '{http://schemas.openxmlformats.org/wordprocessingml/2006/main}sz')
            ssz.set('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}val', str(size))
            
            spPr = ET.SubElement(style, '{http://schemas.openxmlformats.org/wordprocessingml/2006/main}pPr')
            keepNext = ET.SubElement(spPr, '{http://schemas.openxmlformats.org/wordprocessingml/2006/main}keepNext')
            spacing = ET.SubElement(spPr, '{http://schemas.openxmlformats.org/wordprocessingml/2006/main}spacing')
            spacing.set('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}before', '240')
            spacing.set('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}after', '120')
        
        # 创建 relationships
        rels = ET.Element('{http://schemas.openxmlformats.org/package/2006/relationships}Relationships')
        rel = ET.SubElement(rels, '{http://schemas.openxmlformats.org/package/2006/relationships}Relationship')
        rel.set('Id', 'rId1')
        rel.set('Type', 'http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument')
        rel.set('Target', 'word/document.xml')
        
        # 创建 word/_rels/document.xml.rels
        doc_rels = ET.Element('{http://schemas.openxmlformats.org/package/2006/relationships}Relationships')
        doc_rel = ET.SubElement(doc_rels, '{http://schemas.openxmlformats.org/package/2006/relationships}Relationship')
        doc_rel.set('Id', 'rId1')
        doc_rel.set('Type', 'http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles')
        doc_rel.set('Target', 'styles.xml')
        
        # 写入 ZIP
        with ZipFile(output_path, 'w', ZIP_DEFLATED) as zf:
            zf.writestr('[Content_Types].xml', ET.tostring(content_types, xml_declaration=True, encoding='UTF-8'))
            zf.writestr('_rels/.rels', ET.tostring(rels, xml_declaration=True, encoding='UTF-8'))
            zf.writestr('word/document.xml', ET.tostring(document, xml_declaration=True, encoding='UTF-8'))
            zf.writestr('word/_rels/document.xml.rels', ET.tostring(doc_rels, xml_declaration=True, encoding='UTF-8'))
            zf.writestr('word/styles.xml', ET.tostring(styles, xml_declaration=True, encoding='UTF-8'))
        
        return output_path


def convert_md_to_docx(md_path, docx_path):
    """将 Markdown 文件转换为 Word 文档"""
    with open(md_path, 'r', encoding='utf-8') as f:
        md_text = f.read()
    
    parser = MarkdownParser()
    parser.parse(md_text)
    
    docx = DocxGenerator()
    
    for block in parser.blocks:
        if block[0] == 'heading':
            docx.add_heading(block[1], block[2])
        elif block[0] == 'paragraph':
            docx.add_paragraph(block[1])
        elif block[0] == 'code':
            docx.add_code_block(block[1])
        elif block[0] == 'list':
            docx.add_list(block[1])
        elif block[0] == 'ordered_list':
            docx.add_list(block[1], ordered=True)
        elif block[0] == 'table':
            docx.add_table(block[1], block[2])
        elif block[0] == 'hr':
            docx.add_hr()
    
    docx.save(docx_path)
    return docx_path


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("用法: python md_to_docx.py <输入.md> <输出.docx>")
        sys.exit(1)
    
    md_path = sys.argv[1]
    docx_path = sys.argv[2]
    
    if not os.path.exists(md_path):
        print(f"错误: 找不到文件 {md_path}")
        sys.exit(1)
    
    print(f"正在转换: {md_path} → {docx_path}")
    convert_md_to_docx(md_path, docx_path)
    print(f"转换完成: {docx_path}")
