-- clear_data.sql ---
-- -*- SQL[ansi] -*-
-- Copyright (C) 2008 Rex McMaster
-- Author: Rex McMaster rex@mcmaster.id.au
-- 
-- This program is free software; you can redistribute it and/or
-- modify it under the terms of the GNU General Public License as
-- published by the Free Software Foundation; either version 2, or (at
-- your option) any later version.
-- 
-- This program is distributed in the hope that it will be useful, but
-- WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
-- General Public License for more details.
-- 
-- You should have received a copy of the GNU General Public License
-- along with this program; see the file COPYING.  If not, write to
-- the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
-- Boston, MA 02110-1301, USA.

-- reset PRAC database - leaving metadata and basic configuration data
-- 

-- psql -d mills_purge -c "select mtcl_name from mtcl where mtcl_name in  (select tablename from pg_tables) and mtcl_name !~ '^mt*' order by mtcl_name;"
-- awk '{printf("delete from %s where %s_code != \047-\047;\ndelete from %s where %s__sequence != 0;\n", $2, $2, $2, $2)}'
-- awk '{printf("select setval(\047%s_%s__sequence_seq\047, max(%s__sequence) from %s;\n", $3, $3, $3, $3)}'

-- disable triggers
select disable_triggers();

-- clear selected tables
delete from accl where accl_code != '-';
delete from bank where bank_code != '-';
delete from bkdp where bkdp__sequence != 0;
delete from cash where cash__sequence != 0;
delete from cred where cred__sequence != 0;
delete from dbtr where dbtr_code != '-';
delete from docs where docs__sequence != 0;
delete from empl where empl_code != '-';
delete from evnt where evnt__sequence != 0;
delete from feeb where feeb__sequence != 0;
delete from invc where invc__sequence != 0;
delete from note where note__sequence != 0;
delete from pate where pate__sequence != 0;
delete from patn where patn__sequence != 0;
delete from paym where paym__sequence != 0;
delete from prov where prov_code != '-';
delete from rfdr where rfdr_code != '-';
delete from serv where serv_code != '-';
delete from svpf where svpf__sequence != 0;

select enable_triggers();

-- reset sequences
select setval('accl_accl__sequence_seq', coalesce(max(accl__sequence), 1)) from accl;
select setval('bank_bank__sequence_seq', coalesce(max(bank__sequence), 1)) from bank;
select setval('bkdp_bkdp__sequence_seq', coalesce(max(bkdp__sequence), 1)) from bkdp;
select setval('cash_cash__sequence_seq', coalesce(max(cash__sequence), 1)) from cash;
select setval('cred_cred__sequence_seq', coalesce(max(cred__sequence), 1)) from cred;
select setval('dbtr_dbtr__sequence_seq', coalesce(max(dbtr__sequence), 1)) from dbtr;
select setval('docs_docs__sequence_seq', coalesce(max(docs__sequence), 1)) from docs;
select setval('empl_empl__sequence_seq', coalesce(max(empl__sequence), 1)) from empl;
select setval('evnt_evnt__sequence_seq', coalesce(max(evnt__sequence), 1)) from evnt;
select setval('feeb_feeb__sequence_seq', coalesce(max(feeb__sequence), 1)) from feeb;
select setval('invc_invc__sequence_seq', coalesce(max(invc__sequence), 1)) from invc;
select setval('note_note__sequence_seq', coalesce(max(note__sequence), 1)) from note;
select setval('pate_pate__sequence_seq', coalesce(max(pate__sequence), 1)) from pate;
select setval('patn_patn__sequence_seq', coalesce(max(patn__sequence), 1)) from patn;
select setval('paym_paym__sequence_seq', coalesce(max(paym__sequence), 1)) from paym;
select setval('prov_prov__sequence_seq', coalesce(max(prov__sequence), 1)) from prov;
select setval('rfdr_rfdr__sequence_seq', coalesce(max(rfdr__sequence), 1)) from rfdr;
select setval('serv_serv__sequence_seq', coalesce(max(serv__sequence), 1)) from serv;
select setval('svpf_svpf__sequence_seq', coalesce(max(svpf__sequence), 1)) from svpf;


-- end clear_data.sql
