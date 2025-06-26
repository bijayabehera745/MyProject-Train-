import json, csv, os

# Paths
script_dir = os.path.dirname(os.path.abspath(__file__))
json_path = os.path.join(script_dir, "railwayStationsList.json")  # or list_of_stations.json
output_csv = os.path.join(script_dir, "all_station_code_name.csv")

# Load full station list
with open(json_path, encoding='utf-8') as f:
    stations = json.load(f)  # Array of {stnCode, stnName, ...}

# Build lookup map
code_to_name = {s.get("stnCode", s.get("station_code")).upper(): s.get("stnName", s.get("station_name")) for s in stations}

# Your station codes list (400+)
user_codes = """ADI, AGC, AII, AJJ, AJNI, AK, ALNI, AN, ANDN, ANG, ANND, ANVT, APR, ARL, ASN, ASR, ATT, AUS, AWB, AWY, AY, BALU, BAM, BAU, BBK, BBMN, BBS, BC, BCQ, BD, BDHY, BDQ, BDSW, BDTS, BE, BEAS, BH, BHB, BHC, BHI, BHL, BHP, BHRL, BHS, BIDD, BIH, BINA, BJR, BKN, BLS, BLSD, BLU, BMF, BNSR, BNW, BNY, BNZ, BPB, BPL, BPR, BPRD, BRC, BRR, BSB, BSL, BSP, BSR, BST, BTE, BTI, BUGL, BUGY, BVI, BWC, BZA, CAM, CBE, CHE, CHI, CHNR, CKDL, CKP, CNB, CNGR, COR, CPA, CPH, CPJ, CPR, CSMT, CSN, CTC, CTKT, CUR, DAB, DARA, DD, DDV, DEP, DHN, DHT, DIA, DKNT, DLI, DMN, DMO, DNA, DOKM, DOKY, DPA, DPK, DPLN, DPT, DQG, DR, DSA, DSJ, DUE, DUI, DURG, DVD, DZP, ED, ENB, ERN, ET, FBD, FGR, FL, FSW, G, GADJ, GAMI, GD, GHGL, GHJ, GHY, GJS, GKP, GOTN, GWL, GZB, HBD, HD, HJI, HLW, HMH, HMO, HNL, HRNR, HSR, HTE, HW, HWH, HYB, IDH, IGP, IZN, JAC, JACN, JAT, JBP, JES, JHS, JJKR, JKE, JL, JLF, JLWC, JMP, JNU, JOH, JP, JRC, JRKN, JSG, JSM, JSME, JTJ, JU, JUC, JUDW, JWP, KAO, KAP, KAWR, KDS, KES, KESR, KFA, KGG, KGP, KHED, KHH, KHTU, KIW, KJN, KJT, KK, KKDE, KKNA, KKW, KLD, KLJ, KMC, KMME, KMZ, KNE, KNNA, KNW, KNZ, KOAA, KOJ, KOP, KOTA, KPD, KPG, KQW, KRC, KRMI, KSG, KSJ, KT, KTE, KTHU, KTYM, KUDA, KUDL, KUN, KUR, KYJ, KYN, KYQ, LAR, LAU, LDH, LHU, LJN, LKO, LKU, LMG, LNL, LOHA, LTT, MAJN, MAKR, MAO, MAS, MBSK, MDB, MDPB, MDS, MED, MFB, MGR, MGS, MHRG, MIO, MKN, MKU, MKX, MLDT, MML, MMR, MMU, MNE, MNGR, MNI, MOZ, MRJ, MSWA, MTC, MTD, MTJ, MUE, MULK, MUR, MVLK, MYR, MZR, NAD, NBQ, NCB, ND, NDB, NDLS, NED, NFK, NGP, NHGJ, NHR, NJP, NK, NLR, NMH, NOQ, NPS, NRWI, NU, NUH, NZB, NZM, OGL, PAU, PBC, PGT, PIH, PJP, PKA, PKR, PKY, PLJE, PLNI, PLO, PNBE, PND, PNP, PNVL, POU, POY, PPI, PRUR, PSA, PTA, PTK, PTKC, PUNE, PURI, PW, QLN, R, RAA, RAJP, RDT, RE, REN, REWA, RIG, RJPB, RJR, RJT, RJY, RK, RKL, RMA, RN, RNY, ROHA, ROK, ROU, RPH, RPJ, RPO, RSNR, RTGH, RTM, SA, SBDP, SBM, SBV, SC, SCL, SDF, SDGH, SDL, SDLP, SDMK, SDS, SEG, SGNR, SGO, SGR, SHC, SHG, SHR, SJNP, SMBX, SMNN, SMQL, SNDD, SNP, SOG, SOT, SRC, SRE, SRKN, SRPR, SRW, SRZ, SSA, SSW, ST, STA, STN, STR, SUJH, SUNR, SV, SVDK, SVO, SWA, SWM, SWNI, SWV, SZE, TATA, TCR, TDW, THE, THVM, TIBI, TLC, TLI, TNA, TNO, TNW, TOI, TPZ, TRVL, TSD, TUP, TVC, UD, UDT, UDZ, UHP, UJN, ULT, UMB, UMNR, UMR, URK, VAPI, VBW, VG, VJPJ, VSKP, VZM, WHM, WKR, WR, YJUD, ZPR"""  # paste full list
user_codes = [c.strip().upper() for c in user_codes.split(",") if c.strip()]

# Match and prepare output
rows = []
for code in user_codes:
    name = code_to_name.get(code, "UNKNOWN")
    rows.append((code, name))

# Save CSV
with open(output_csv, "w", newline='', encoding='utf-8') as out:
    writer = csv.writer(out)
    writer.writerow(["Station Code", "Station Name"])
    writer.writerows(rows)

print(f"✅ Saved {len(rows)} entries to:\n{output_csv}")
print(f"Unmatched codes: {[c for c,n in rows if n=='UNKNOWN']}")
