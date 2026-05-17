#!/usr/bin/env python3
"""
HIS 测试数据生成脚本
所有数据完全固定（无随机），日期基于当天生成，挂号均为未来日期
"""

import os
import time
from datetime import datetime, timedelta

DATA_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "data")
os.makedirs(DATA_DIR, exist_ok=True)

# ============================================================
# 工具函数
# ============================================================

def cipher_xor_cyclic(data: str) -> bytes:
    key = b"HIS_XOR"
    raw = data.encode("utf-8")
    result = bytearray(32)
    for i in range(len(raw)):
        result[i] = raw[i] ^ key[i % len(key)]
    return bytes(result)

def make_timestamp(year, month, day, hour=0, minute=0, second=0):
    """生成 Unix 时间戳"""
    days_in_month = [0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    def is_leap(y):
        return (y % 400 == 0) or (y % 4 == 0 and y % 100 != 0)
    total_days = 0
    for y in range(1970, year):
        total_days += 366 if is_leap(y) else 365
    for m in range(1, month):
        total_days += days_in_month[m]
        if m == 2 and is_leap(year):
            total_days += 1
    total_days += day - 1
    return total_days * 86400 + hour * 3600 + minute * 60 + second

def int_date(year, month, day):
    return year * 10000 + month * 100 + day

def now_ts():
    return int(time.time())

# 获取当天日期
today = datetime.now()
TODAY_YEAR = today.year
TODAY_MONTH = today.month
TODAY_DAY = today.day
TODAY_INT = int_date(TODAY_YEAR, TODAY_MONTH, TODAY_DAY)
TODAY_TS = int(today.timestamp())

# 未来几天的日期
def future_date(days_later):
    d = today + timedelta(days=days_later)
    return int_date(d.year, d.month, d.day)

def future_ts(days_later, hour=8, minute=0, second=0):
    d = today + timedelta(days=days_later)
    return make_timestamp(d.year, d.month, d.day, hour, minute, second)

# 过去几天的日期（用于诊疗行为）
def past_ts(days_ago, hour=8, minute=0, second=0):
    d = today - timedelta(days=days_ago)
    return make_timestamp(d.year, d.month, d.day, hour, minute, second)

# ============================================================
# 固定数据定义
# ============================================================

DEPARTMENTS = [
    (1, "内科"), (2, "外科"), (3, "儿科"), (4, "妇产科"),
    (5, "眼科"), (6, "口腔科"), (7, "皮肤科"), (8, "急诊科"),
    (9, "放射科"), (10, "检验科"), (11, "药剂科")
]

def department_name(dept_id):
    for d in DEPARTMENTS:
        if d[0] == dept_id:
            return d[1]
    return "未知"

# 医生数据（固定）
DOCTOR_DATA = [
    # (id, name, gender, birth_ts, dept_id, title, reg_fee, phone, id_card)
    (1, "王建国", 1, make_timestamp(1975, 3, 15), 1, 3, 5000, "13800138001", "110101197503151234"),
    (2, "李明芳", 0, make_timestamp(1982, 7, 22), 1, 2, 3000, "13800138002", "110101198207221235"),
    (3, "张伟",   1, make_timestamp(1970, 11, 8), 2, 3, 6000, "13800138003", "110101197011081236"),
    (4, "刘洋",   0, make_timestamp(1985, 5, 12), 2, 2, 3500, "13800138004", "110101198505121237"),
    (5, "陈静",   0, make_timestamp(1978, 9, 1),  3, 3, 4000, "13800138005", "110101197809011238"),
    (6, "赵强",   1, make_timestamp(1990, 2, 18), 3, 1, 2000, "13800138006", "110101199002181239"),
    (7, "孙丽华", 0, make_timestamp(1972, 6, 30), 4, 3, 5500, "13800138007", "110101197206301240"),
    (8, "周敏",   0, make_timestamp(1988, 12, 5), 4, 2, 3000, "13800138008", "110101198812051241"),
    (9, "吴明",   1, make_timestamp(1980, 4, 20), 5, 2, 3500, "13800138009", "110101198004201242"),
    (10,"郑小红", 0, make_timestamp(1992, 8, 15), 5, 1, 2000, "13800138010", "110101199208151243"),
    (11,"黄磊",   1, make_timestamp(1976, 1, 10), 6, 3, 4500, "13800138011", "110101197601101244"),
    (12,"林婷婷", 0, make_timestamp(1989, 3, 25), 6, 2, 3000, "13800138012", "110101198903251245"),
    (13,"何志远", 1, make_timestamp(1983, 10, 5), 7, 2, 3000, "13800138013", "110101198310051246"),
    (14,"马晓燕", 0, make_timestamp(1991, 7, 18), 7, 1, 2000, "13800138014", "110101199107181247"),
    (15,"高峰",   1, make_timestamp(1973, 12, 28),8, 3, 5000, "13800138015", "110101197312281248"),
    (16,"杨雪",   0, make_timestamp(1986, 4, 8),  8, 2, 3500, "13800138016", "110101198604081249"),
    (17,"徐大伟", 1, make_timestamp(1979, 8, 14), 9, 2, 3000, "13800138017", "110101197908141250"),
    (18,"唐小芳", 0, make_timestamp(1993, 11, 2), 9, 1, 2000, "13800138018", "110101199311021251"),
    (19,"沈国平", 1, make_timestamp(1977, 5, 9),  10,2, 2500, "13800138019", "110101197705091252"),
    (20,"顾美琴", 0, make_timestamp(1984, 9, 16), 10,2, 2500, "13800138020", "110101198409161253"),
    (21,"钱进",   1, make_timestamp(1974, 2, 28), 11,3, 0,    "13800138021", "110101197402281254"),
    (22,"韩冰",   0, make_timestamp(1987, 6, 11), 11,2, 0,    "13800138022", "110101198706111255"),
]

# 病人数据（固定）
PATIENT_DATA = [
    # (id, name, gender, birth_ts, phone, id_card)
    (1,  "赵大宝", 1, make_timestamp(1995, 3, 10),  "13900001001", "320101199503101234"),
    (2,  "钱小花", 0, make_timestamp(1998, 7, 22),  "13900001002", "320101199807221235"),
    (3,  "孙铁柱", 1, make_timestamp(1988, 11, 5),  "13900001003", "320101198811051236"),
    (4,  "李翠花", 0, make_timestamp(2000, 1, 15),  "13900001004", "320101200001151237"),
    (5,  "周大牛", 1, make_timestamp(1975, 6, 30),  "13900001005", "320101197506301238"),
    (6,  "吴小燕", 0, make_timestamp(2002, 9, 8),   "13900001006", "320101200209081239"),
    (7,  "郑明",   1, make_timestamp(1992, 4, 18),  "13900001007", "320101199204181240"),
    (8,  "王芳",   0, make_timestamp(1985, 12, 25), "13900001008", "320101198512251241"),
    (9,  "冯刚",   1, make_timestamp(1970, 8, 12),  "13900001009", "320101197008121242"),
    (10, "陈小红", 0, make_timestamp(1996, 2, 28),  "13900001010", "320101199602281243"),
    (11, "褚卫东", 1, make_timestamp(1980, 5, 3),   "13900001011", "320101198005031244"),
    (12, "卫兰",   0, make_timestamp(1993, 10, 20), "13900001012", "320101199310201245"),
    (13, "蒋天宇", 1, make_timestamp(2001, 7, 14),  "13900001013", "320101200107141246"),
    (14, "沈佳宜", 0, make_timestamp(1999, 1, 1),   "13900001014", "320101199901011247"),
    (15, "韩磊",   1, make_timestamp(1978, 3, 22),  "13900001015", "320101197803221248"),
    (16, "杨玉环", 0, make_timestamp(2003, 6, 6),   "13900001016", "320101200306061249"),
    (17, "朱洪武", 1, make_timestamp(1965, 9, 9),   "13900001017", "320101196509091250"),
    (18, "秦淑仪", 0, make_timestamp(1990, 11, 11), "13900001018", "320101199011111251"),
    (19, "许仙",   1, make_timestamp(1982, 4, 4),   "13900001019", "320101198204041252"),
    (20, "白素贞", 0, make_timestamp(1986, 12, 12), "13900001020", "320101198612121253"),
]

# 药品数据（固定）
MEDICINE_DATA = [
    # (name, cur_price, batches)
    # batches: [(buy_price, expire_days, remain, batch_no), ...]
    ("阿莫西林胶囊", 1500, [
        (800, 730, 200, "AMX2025A001"),
        (850, 500, 150, "AMX2025B002"),
    ]),
    ("布洛芬缓释片", 1200, [
        (600, 600, 300, "BLF2025A001"),
    ]),
    ("头孢克肟分散片", 2800, [
        (1800, 400, 100, "TBKW2025A001"),
        (1750, 350, 80, "TBKW2025B002"),
    ]),
    ("蒙脱石散", 800, [
        (400, 900, 500, "MTS2025A001"),
    ]),
    ("氯雷他定片", 900, [
        (500, 550, 250, "LLTD2025A001"),
    ]),
    ("盐酸氨溴索片", 600, [
        (300, 800, 400, "YSAXS2025A001"),
    ]),
    ("硝苯地平缓释片", 2200, [
        (1400, 650, 180, "XBDP2025A001"),
    ]),
    ("奥美拉唑肠溶胶囊", 1800, [
        (1000, 700, 220, "AMLZ2025A001"),
    ]),
    ("阿司匹林肠溶片", 500, [
        (250, 1000, 600, "ASPL2025A001"),
    ]),
    ("维生素C片", 300, [
        (100, 1095, 1000, "Vc2025A001"),
    ]),
]

# 病房数据（固定）
WARD_DATA = [
    # (id, dept_id, daily_cost, start_bed, bed_count, name)
    (1, 1, 5000, 1, 10, "内科一病区"),
    (2, 1, 4500, 11, 8, "内科二病区"),
    (3, 2, 6000, 1, 10, "外科一病区"),
    (4, 2, 5500, 11, 8, "外科二病区"),
    (5, 3, 4000, 1, 6, "儿科病区"),
    (6, 4, 5000, 1, 8, "妇产科病区"),
    (7, 8, 8000, 1, 5, "急诊观察区"),
]

# 资金账户余额（固定，单位：分）
FUND_BALANCES = {
    1: 201210, 2: 438533, 3: 309883, 4: 320654, 5: 322023,
    6: 372212, 7: 408616, 8: 238707, 9: 382570, 10: 439940,
    11: 470903, 12: 79403, 13: 240711, 14: 216176, 15: 462185,
    16: 279851, 17: 310470, 18: 447842, 19: 445926, 20: 189304,
}

# 病人-医生关系（固定）
PATIENT_DOCTOR_REL = {
    1: 1, 2: 1, 3: 2, 4: 2, 5: 3, 6: 3, 7: 4, 8: 4,
    9: 5, 10: 5, 11: 6, 12: 6, 13: 7, 14: 7, 15: 8,
    16: 8, 17: 9, 18: 9, 19: 10, 20: 10,
}

# 住院病人关系（固定）
# (pat_id, ward_id, bed_label, admit_ts)
PATIENT_WARD_REL = {
    5:  (3, 1, future_ts(1, 11, 0)),    # 周大牛 → 外科一病区 床1
    11: (1, 11, future_ts(2, 16, 0)),   # 褚卫东 → 内科一病区 床11
    17: (6, 1, future_ts(4, 10, 0)),    # 朱洪武 → 妇产科病区 床1
}

# ============================================================
# 生成 Patient.txt
# ============================================================
def gen_patient():
    lines = ["ID|Gender|Birth_TS|Name|Phone|ID_Card"]
    for pid, name, gender, birth_ts, phone, id_card in PATIENT_DATA:
        lines.append(f"{pid}|{gender}|{birth_ts}|{name}|{phone}|{id_card}")
    with open(os.path.join(DATA_DIR, "Patient.txt"), "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print(f"[OK] Patient.txt: {len(PATIENT_DATA)} 条记录")

# ============================================================
# 生成 Doctor.txt
# ============================================================
def gen_doctor():
    lines = ["ID|Gender|Birth_TS|Is_Active|Dept|Title|Name|Phone|ID_Card|Reg_Fee"]
    for did, name, gender, birth_ts, dept, title, reg_fee, phone, id_card in DOCTOR_DATA:
        lines.append(f"{did}|{gender}|{birth_ts}|1|{dept}|{title}|{name}|{phone}|{id_card}|{reg_fee}")
    with open(os.path.join(DATA_DIR, "Doctor.txt"), "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print(f"[OK] Doctor.txt: {len(DOCTOR_DATA)} 条记录")

# ============================================================
# 生成 Account.txt
# ============================================================
def gen_account():
    lines = ["Class|Actor_id|Name|Password"]
    enc_root = cipher_xor_cyclic("root")
    hex_root = "".join(f"{b:02X}" for b in enc_root)
    lines.append(f"3|0|root|{hex_root}")

    enc_pat = cipher_xor_cyclic("00000000")
    hex_pat = "".join(f"{b:02X}" for b in enc_pat)
    for pid, name, _, _, _, _ in PATIENT_DATA:
        lines.append(f"1|{pid}|{name}|{hex_pat}")

    enc_doc = cipher_xor_cyclic("88888888")
    hex_doc = "".join(f"{b:02X}" for b in enc_doc)
    for did, name, _, _, _, _, _, _, _ in DOCTOR_DATA:
        lines.append(f"2|{did}|{name}|{hex_doc}")

    with open(os.path.join(DATA_DIR, "Account.txt"), "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    total = 1 + len(PATIENT_DATA) + len(DOCTOR_DATA)
    print(f"[OK] Account.txt: {total} 条记录")

# ============================================================
# 生成 Fund.txt
# ============================================================
def gen_fund():
    lines = ["Pat_id|Balance"]
    for pid in range(1, len(PATIENT_DATA) + 1):
        lines.append(f"{pid}|{FUND_BALANCES[pid]}")
    with open(os.path.join(DATA_DIR, "Fund.txt"), "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print(f"[OK] Fund.txt: {len(PATIENT_DATA)} 条记录")

# ============================================================
# 生成 Medicine.txt
# ============================================================
def gen_medicine():
    lines = ["Med_id|Cur_price|Total_remain|Name"]
    batch_id_counter = 1
    for i, (name, cur_price, batches) in enumerate(MEDICINE_DATA, start=1):
        total_remain = sum(b[2] for b in batches)
        lines.append(f"M|{i}|{cur_price}|{total_remain}|{name}")
        for batch in batches:
            buy_price, expire_days, remain, batch_no = batch
            expire_ts = now_ts() + expire_days * 86400
            status = 0  # AVAILABLE
            lines.append(f"B|{batch_id_counter}|{buy_price}|{expire_ts}|{remain}|{status}|{batch_no}")
            batch_id_counter += 1
    with open(os.path.join(DATA_DIR, "Medicine.txt"), "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print(f"[OK] Medicine.txt: {len(MEDICINE_DATA)} 种药品")

# ============================================================
# 生成 Ward.txt
# ============================================================
def gen_ward():
    lines = ["Ward_id|Dept|Daily_Cost|Bed_Count|Empty_Count|Ward_Name"]
    # 构建床位占用表: (ward_id, bed_label) -> (pat_id, admit_ts)
    occupied_beds = {}
    for pat_id, (ward_id, bed_label, admit_ts) in PATIENT_WARD_REL.items():
        occupied_beds[(ward_id, bed_label)] = (pat_id, admit_ts)

    for wid, dept, daily_cost, start_bed, bed_count, name in WARD_DATA:
        # 计算空床数
        empty_count = bed_count
        for bed_label in range(start_bed, start_bed + bed_count):
            if (wid, bed_label) in occupied_beds:
                empty_count -= 1
        lines.append(f"W|{wid}|{dept}|{daily_cost}|{bed_count}|{empty_count}|{name}")

        for bed_label in range(start_bed, start_bed + bed_count):
            if (wid, bed_label) in occupied_beds:
                pat_id, admit_ts = occupied_beds[(wid, bed_label)]
                lines.append(f"B|{bed_label}|{pat_id}|1|{admit_ts}|")
            else:
                lines.append(f"B|{bed_label}|0|0|0|")
    with open(os.path.join(DATA_DIR, "Ward.txt"), "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print(f"[OK] Ward.txt: {len(WARD_DATA)} 间病房")

# ============================================================
# 生成 Record.txt
# ============================================================
def gen_record():
    lines = ["Type|Is_Invalid|Actor_Id|Time_Stamp|Cost|Detail..."]
    day_sec = 86400
    rec_id = 0

    # 每个病人的固定记录
    # 原则：预约挂号用未来时间，诊疗行为（看诊/开药/检查/住院/出院）用过去时间

    # 病人1 赵大宝 → 医生1 王建国（内科）
    # 挂号（当天，未来）
    rec_id += 1
    reg_ts = future_ts(0, 8, 30)
    reg_date = future_date(0)
    lines.append(f"0|0|1|{reg_ts}|0|1|{rec_id}|{reg_date}|1|0")
    # 看诊（过去1天）
    rec_id += 1
    cons_ts = past_ts(1, 9, 15)
    lines.append(f"1|0|1|{cons_ts}|5000|1|上呼吸道感染|多休息多喝水")
    # 开药（过去1天）
    rec_id += 1
    pres_ts = past_ts(1, 9, 45)
    lines.append(f"3|0|1|{pres_ts}|3000|1|1|2")

    # 病人2 钱小花 → 医生1 王建国（内科）
    rec_id += 1
    reg_ts = future_ts(0, 9, 0)
    reg_date = future_date(0)
    lines.append(f"0|0|2|{reg_ts}|0|1|{rec_id}|{reg_date}|2|0")
    rec_id += 1
    cons_ts = past_ts(1, 9, 50)
    lines.append(f"1|0|2|{cons_ts}|5000|1|胃炎|注意饮食清淡")
    rec_id += 1
    pres_ts = past_ts(1, 10, 20)
    lines.append(f"3|0|2|{pres_ts}|2400|1|8|2")

    # 病人3 孙铁柱 → 医生2 李明芳（内科）
    rec_id += 1
    reg_ts = future_ts(0, 10, 0)
    reg_date = future_date(0)
    lines.append(f"0|0|3|{reg_ts}|0|2|{rec_id}|{reg_date}|3|0")
    rec_id += 1
    cons_ts = past_ts(1, 10, 40)
    lines.append(f"1|0|3|{cons_ts}|10000|2|高血压|按时服药，定期复查")
    rec_id += 1
    pres_ts = past_ts(1, 11, 10)
    lines.append(f"3|0|3|{pres_ts}|5600|2|7|2")

    # 病人4 李翠花 → 医生2 李明芳（内科）
    rec_id += 1
    reg_ts = future_ts(0, 14, 0)
    reg_date = future_date(0)
    lines.append(f"0|0|4|{reg_ts}|0|2|{rec_id}|{reg_date}|10|0")

    # 病人5 周大牛 → 医生3 张伟（外科）→ 住院
    rec_id += 1
    reg_ts = future_ts(0, 8, 0)
    reg_date = future_date(0)
    lines.append(f"0|0|5|{reg_ts}|0|3|{rec_id}|{reg_date}|0|0")
    rec_id += 1
    cons_ts = past_ts(1, 8, 40)
    lines.append(f"1|0|5|{cons_ts}|10000|3|腰椎间盘突出|建议住院观察")
    rec_id += 1
    exam_ts = past_ts(1, 9, 30)
    lines.append(f"2|0|5|{exam_ts}|12000|3|CT扫描")
    rec_id += 1
    admit_ts = past_ts(1, 11, 0)
    lines.append(f"4|0|5|{admit_ts}|0|3|1|100000")

    # 病人6 吴小燕 → 医生3 张伟（外科）
    rec_id += 1
    reg_ts = future_ts(0, 15, 0)
    reg_date = future_date(0)
    lines.append(f"0|0|6|{reg_ts}|0|3|{rec_id}|{reg_date}|13|0")

    # 病人7 郑明 → 医生4 刘洋（外科）
    rec_id += 1
    reg_ts = future_ts(2, 8, 0)
    reg_date = future_date(2)
    lines.append(f"0|0|7|{reg_ts}|0|4|{rec_id}|{reg_date}|0|0")

    # 病人8 王芳 → 医生4 刘洋（外科）
    rec_id += 1
    reg_ts = future_ts(2, 9, 0)
    reg_date = future_date(2)
    lines.append(f"0|0|8|{reg_ts}|0|4|{rec_id}|{reg_date}|2|0")

    # 病人9 冯刚 → 医生5 陈静（儿科）
    rec_id += 1
    reg_ts = future_ts(2, 8, 30)
    reg_date = future_date(2)
    lines.append(f"0|0|9|{reg_ts}|0|5|{rec_id}|{reg_date}|1|0")
    rec_id += 1
    cons_ts = past_ts(2, 9, 10)
    lines.append(f"1|0|9|{cons_ts}|5000|5|上呼吸道感染|多休息，多喝水")
    rec_id += 1
    pres_ts = past_ts(2, 9, 40)
    lines.append(f"3|0|9|{pres_ts}|1500|5|1|1")

    # 病人10 陈小红 → 医生5 陈静（儿科）
    rec_id += 1
    reg_ts = future_ts(2, 10, 0)
    reg_date = future_date(2)
    lines.append(f"0|0|10|{reg_ts}|0|5|{rec_id}|{reg_date}|3|0")

    # 病人11 褚卫东 → 医生6 赵强（儿科）→ 住院
    rec_id += 1
    reg_ts = future_ts(2, 14, 0)
    reg_date = future_date(2)
    lines.append(f"0|0|11|{reg_ts}|0|6|{rec_id}|{reg_date}|10|0")
    rec_id += 1
    cons_ts = past_ts(2, 14, 40)
    lines.append(f"1|0|11|{cons_ts}|10000|6|糖尿病|按时服药，定期复查")
    rec_id += 1
    admit_ts = past_ts(2, 16, 0)
    lines.append(f"4|0|11|{admit_ts}|0|1|11|80000")

    # 病人12 卫兰 → 医生6 赵强（儿科）
    rec_id += 1
    reg_ts = future_ts(3, 8, 0)
    reg_date = future_date(3)
    lines.append(f"0|0|12|{reg_ts}|0|6|{rec_id}|{reg_date}|0|0")

    # 病人13 蒋天宇 → 医生7 孙丽华（妇产科）
    rec_id += 1
    reg_ts = future_ts(3, 8, 30)
    reg_date = future_date(3)
    lines.append(f"0|0|13|{reg_ts}|0|7|{rec_id}|{reg_date}|1|0")

    # 病人14 沈佳宜 → 医生7 孙丽华（妇产科）
    rec_id += 1
    reg_ts = future_ts(3, 9, 30)
    reg_date = future_date(3)
    lines.append(f"0|0|14|{reg_ts}|0|7|{rec_id}|{reg_date}|3|0")

    # 病人15 韩磊 → 医生8 周敏（妇产科）
    rec_id += 1
    reg_ts = future_ts(3, 10, 0)
    reg_date = future_date(3)
    lines.append(f"0|0|15|{reg_ts}|0|8|{rec_id}|{reg_date}|4|0")

    # 病人16 杨玉环 → 医生8 周敏（妇产科）
    rec_id += 1
    reg_ts = future_ts(3, 14, 0)
    reg_date = future_date(3)
    lines.append(f"0|0|16|{reg_ts}|0|8|{rec_id}|{reg_date}|10|0")

    # 病人17 朱洪武 → 医生9 吴明（眼科）→ 住院
    rec_id += 1
    reg_ts = future_ts(4, 8, 0)
    reg_date = future_date(4)
    lines.append(f"0|0|17|{reg_ts}|0|9|{rec_id}|{reg_date}|0|0")
    rec_id += 1
    cons_ts = past_ts(4, 8, 35)
    lines.append(f"1|0|17|{cons_ts}|5000|9|结膜炎|按时服药，定期复查")
    rec_id += 1
    admit_ts = past_ts(4, 10, 0)
    lines.append(f"4|0|17|{admit_ts}|0|6|1|50000")

    # 病人18 秦淑仪 → 医生9 吴明（眼科）
    rec_id += 1
    reg_ts = future_ts(4, 9, 0)
    reg_date = future_date(4)
    lines.append(f"0|0|18|{reg_ts}|0|9|{rec_id}|{reg_date}|2|0")

    # 病人19 许仙 → 医生10 郑小红（眼科）
    rec_id += 1
    reg_ts = future_ts(4, 10, 0)
    reg_date = future_date(4)
    lines.append(f"0|0|19|{reg_ts}|0|10|{rec_id}|{reg_date}|4|0")

    # 病人20 白素贞 → 医生10 郑小红（眼科）
    rec_id += 1
    reg_ts = future_ts(4, 14, 0)
    reg_date = future_date(4)
    lines.append(f"0|0|20|{reg_ts}|0|10|{rec_id}|{reg_date}|10|0")

    # 一些历史记录（过去几天的）
    # 病人5 周大牛 出院
    rec_id += 1
    disc_ts = past_ts(1, 17, 0)
    lines.append(f"5|0|5|{disc_ts}|0|120000|100000")

    # 病人11 褚卫东 出院
    rec_id += 1
    disc_ts = past_ts(2, 17, 0)
    lines.append(f"5|0|11|{disc_ts}|0|90000|80000")

    with open(os.path.join(DATA_DIR, "Record.txt"), "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print(f"[OK] Record.txt: {len(lines) - 1} 条记录")

# ============================================================
# 生成 Doctor-relation.txt
# ============================================================
def gen_doctor_relation():
    lines = ["Pat_id|Doc_id"]
    for pid in range(1, len(PATIENT_DATA) + 1):
        lines.append(f"{pid}|{PATIENT_DOCTOR_REL[pid]}")
    with open(os.path.join(DATA_DIR, "Doctor-relation.txt"), "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print(f"[OK] Doctor-relation.txt: {len(PATIENT_DATA)} 条记录")

# ============================================================
# 生成 Ward-relation.txt
# ============================================================
def gen_ward_relation():
    lines = ["Pat_id|Ward_id"]
    for pid, wid in PATIENT_WARD_REL.items():
        lines.append(f"{pid}|{wid}")
    with open(os.path.join(DATA_DIR, "Ward-relation.txt"), "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print(f"[OK] Ward-relation.txt: {len(PATIENT_WARD_REL)} 条记录")

# ============================================================
# 主函数
# ============================================================
def main():
    print("=" * 50)
    print("HIS 测试数据生成器（固定数据版）")
    print(f"基准日期：{TODAY_YEAR}年{TODAY_MONTH}月{TODAY_DAY}日")
    print("=" * 50)
    gen_patient()
    gen_doctor()
    gen_account()
    gen_fund()
    gen_medicine()
    gen_ward()
    gen_record()
    gen_doctor_relation()
    gen_ward_relation()
    print("=" * 50)
    print(f"所有数据已生成到: {DATA_DIR}")
    print("请将 data 文件夹复制到 build/ 目录下覆盖原有数据")
    print("=" * 50)

if __name__ == "__main__":
    main()
