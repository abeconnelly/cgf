// query.js
//

print = ((typeof(print)==="undefined") ? console.log : print);

var cgf_info = {};

function setup_cgf_info() {
  cgf_info.cgf = [];
  cgf_info.cgf.push({ "file":"/data/cgf/HG00403-GS000016660-ASM.cgf", "name":"HG00403-GS000016660-ASM", "id":0 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00404-GS000016669-ASM.cgf", "name":"HG00404-GS000016669-ASM", "id":1 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00405-GS000016661-ASM.cgf", "name":"HG00405-GS000016661-ASM", "id":2 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00406-GS000016662-ASM.cgf", "name":"HG00406-GS000016662-ASM", "id":3 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00407-GS000016663-ASM.cgf", "name":"HG00407-GS000016663-ASM", "id":4 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00408-GS000016670-ASM.cgf", "name":"HG00408-GS000016670-ASM", "id":5 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00421-GS000016832-ASM.cgf", "name":"HG00421-GS000016832-ASM", "id":6 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00422-GS000016833-ASM.cgf", "name":"HG00422-GS000016833-ASM", "id":7 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00423-GS000016671-ASM.cgf", "name":"HG00423-GS000016671-ASM", "id":8 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00436-GS000016683-ASM.cgf", "name":"HG00436-GS000016683-ASM", "id":9 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00437-GS000016672-ASM.cgf", "name":"HG00437-GS000016672-ASM", "id":10 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00438-GS000016673-ASM.cgf", "name":"HG00438-GS000016673-ASM", "id":11 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00442-GS000016869-ASM.cgf", "name":"HG00442-GS000016869-ASM", "id":12 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00443-GS000016674-ASM.cgf", "name":"HG00443-GS000016674-ASM", "id":13 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00444-GS000016675-ASM.cgf", "name":"HG00444-GS000016675-ASM", "id":14 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00448-GS000016676-ASM.cgf", "name":"HG00448-GS000016676-ASM", "id":15 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00449-GS000016677-ASM.cgf", "name":"HG00449-GS000016677-ASM", "id":16 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00450-GS000016678-ASM.cgf", "name":"HG00450-GS000016678-ASM", "id":17 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00463-GS000016679-ASM.cgf", "name":"HG00463-GS000016679-ASM", "id":18 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00464-GS000016680-ASM.cgf", "name":"HG00464-GS000016680-ASM", "id":19 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00465-GS000016681-ASM.cgf", "name":"HG00465-GS000016681-ASM", "id":20 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00472-GS000016706-ASM.cgf", "name":"HG00472-GS000016706-ASM", "id":21 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00473-GS000016707-ASM.cgf", "name":"HG00473-GS000016707-ASM", "id":22 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00474-GS000016708-ASM.cgf", "name":"HG00474-GS000016708-ASM", "id":23 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00475-GS000016709-ASM.cgf", "name":"HG00475-GS000016709-ASM", "id":24 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00476-GS000016710-ASM.cgf", "name":"HG00476-GS000016710-ASM", "id":25 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00477-GS000016711-ASM.cgf", "name":"HG00477-GS000016711-ASM", "id":26 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00478-GS000016712-ASM.cgf", "name":"HG00478-GS000016712-ASM", "id":27 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00479-GS000016713-ASM.cgf", "name":"HG00479-GS000016713-ASM", "id":28 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00480-GS000016714-ASM.cgf", "name":"HG00480-GS000016714-ASM", "id":29 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00530-GS000016715-ASM.cgf", "name":"HG00530-GS000016715-ASM", "id":30 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00531-GS000016849-ASM.cgf", "name":"HG00531-GS000016849-ASM", "id":31 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00532-GS000016716-ASM.cgf", "name":"HG00532-GS000016716-ASM", "id":32 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00533-GS000016717-ASM.cgf", "name":"HG00533-GS000016717-ASM", "id":33 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00534-GS000016718-ASM.cgf", "name":"HG00534-GS000016718-ASM", "id":34 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00535-GS000016719-ASM.cgf", "name":"HG00535-GS000016719-ASM", "id":35 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00536-GS000017031-ASM.cgf", "name":"HG00536-GS000017031-ASM", "id":36 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00537-GS000016720-ASM.cgf", "name":"HG00537-GS000016720-ASM", "id":37 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00538-GS000016721-ASM.cgf", "name":"HG00538-GS000016721-ASM", "id":38 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00542-GS000016722-ASM.cgf", "name":"HG00542-GS000016722-ASM", "id":39 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00543-GS000016723-ASM.cgf", "name":"HG00543-GS000016723-ASM", "id":40 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00544-GS000016724-ASM.cgf", "name":"HG00544-GS000016724-ASM", "id":41 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00556-GS000016725-ASM.cgf", "name":"HG00556-GS000016725-ASM", "id":42 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00557-GS000016845-ASM.cgf", "name":"HG00557-GS000016845-ASM", "id":43 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00558-GS000016834-ASM.cgf", "name":"HG00558-GS000016834-ASM", "id":44 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00559-GS000016835-ASM.cgf", "name":"HG00559-GS000016835-ASM", "id":45 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00560-GS000016830-ASM.cgf", "name":"HG00560-GS000016830-ASM", "id":46 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00561-GS000016836-ASM.cgf", "name":"HG00561-GS000016836-ASM", "id":47 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00589-GS000016837-ASM.cgf", "name":"HG00589-GS000016837-ASM", "id":48 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00590-GS000016838-ASM.cgf", "name":"HG00590-GS000016838-ASM", "id":49 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00591-GS000016839-ASM.cgf", "name":"HG00591-GS000016839-ASM", "id":50 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00592-GS000016840-ASM.cgf", "name":"HG00592-GS000016840-ASM", "id":51 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00593-GS000016841-ASM.cgf", "name":"HG00593-GS000016841-ASM", "id":52 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00594-GS000016842-ASM.cgf", "name":"HG00594-GS000016842-ASM", "id":53 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00607-GS000016843-ASM.cgf", "name":"HG00607-GS000016843-ASM", "id":54 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00608-GS000016831-ASM.cgf", "name":"HG00608-GS000016831-ASM", "id":55 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00609-GS000016844-ASM.cgf", "name":"HG00609-GS000016844-ASM", "id":56 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00610-GS000016850-ASM.cgf", "name":"HG00610-GS000016850-ASM", "id":57 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00611-GS000016851-ASM.cgf", "name":"HG00611-GS000016851-ASM", "id":58 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00612-GS000016852-ASM.cgf", "name":"HG00612-GS000016852-ASM", "id":59 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00613-GS000016853-ASM.cgf", "name":"HG00613-GS000016853-ASM", "id":60 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00614-GS000016854-ASM.cgf", "name":"HG00614-GS000016854-ASM", "id":61 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00615-GS000016855-ASM.cgf", "name":"HG00615-GS000016855-ASM", "id":62 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00619-GS000016979-ASM.cgf", "name":"HG00619-GS000016979-ASM", "id":63 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00620-GS000017245-ASM.cgf", "name":"HG00620-GS000017245-ASM", "id":64 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00621-GS000017116-ASM.cgf", "name":"HG00621-GS000017116-ASM", "id":65 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00625-GS000017120-ASM.cgf", "name":"HG00625-GS000017120-ASM", "id":66 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00626-GS000017125-ASM.cgf", "name":"HG00626-GS000017125-ASM", "id":67 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00627-GS000016860-ASM.cgf", "name":"HG00627-GS000016860-ASM", "id":68 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00628-GS000016861-ASM.cgf", "name":"HG00628-GS000016861-ASM", "id":69 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00629-GS000016862-ASM.cgf", "name":"HG00629-GS000016862-ASM", "id":70 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00630-GS000016863-ASM.cgf", "name":"HG00630-GS000016863-ASM", "id":71 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00650-GS000016864-ASM.cgf", "name":"HG00650-GS000016864-ASM", "id":72 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00651-GS000016865-ASM.cgf", "name":"HG00651-GS000016865-ASM", "id":73 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00652-GS000017129-ASM.cgf", "name":"HG00652-GS000017129-ASM", "id":74 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00653-GS000016867-ASM.cgf", "name":"HG00653-GS000016867-ASM", "id":75 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00654-GS000016868-ASM.cgf", "name":"HG00654-GS000016868-ASM", "id":76 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00655-GS000016980-ASM.cgf", "name":"HG00655-GS000016980-ASM", "id":77 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00662-GS000016981-ASM.cgf", "name":"HG00662-GS000016981-ASM", "id":78 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00663-GS000016983-ASM.cgf", "name":"HG00663-GS000016983-ASM", "id":79 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00664-GS000016984-ASM.cgf", "name":"HG00664-GS000016984-ASM", "id":80 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00671-GS000019202-ASM.cgf", "name":"HG00671-GS000019202-ASM", "id":81 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00672-GS000016985-ASM.cgf", "name":"HG00672-GS000016985-ASM", "id":82 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00673-GS000016986-ASM.cgf", "name":"HG00673-GS000016986-ASM", "id":83 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00683-GS000016987-ASM.cgf", "name":"HG00683-GS000016987-ASM", "id":84 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00684-GS000016988-ASM.cgf", "name":"HG00684-GS000016988-ASM", "id":85 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00685-GS000016978-ASM.cgf", "name":"HG00685-GS000016978-ASM", "id":86 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00689-GS000016977-ASM.cgf", "name":"HG00689-GS000016977-ASM", "id":87 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00690-GS000016994-ASM.cgf", "name":"HG00690-GS000016994-ASM", "id":88 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00691-GS000016989-ASM.cgf", "name":"HG00691-GS000016989-ASM", "id":89 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00692-GS000016991-ASM.cgf", "name":"HG00692-GS000016991-ASM", "id":90 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00693-GS000016992-ASM.cgf", "name":"HG00693-GS000016992-ASM", "id":91 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00694-GS000016993-ASM.cgf", "name":"HG00694-GS000016993-ASM", "id":92 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00731-GS000012103-ASM.cgf", "name":"HG00731-GS000012103-ASM", "id":93 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00731-GS000016547-ASM.cgf", "name":"HG00731-GS000016547-ASM", "id":94 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00732-GS000012104-ASM.cgf", "name":"HG00732-GS000012104-ASM", "id":95 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00732-GS000017235-ASM.cgf", "name":"HG00732-GS000017235-ASM", "id":96 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00733-GS000012105-ASM.cgf", "name":"HG00733-GS000012105-ASM", "id":97 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG00733-GS000016548-ASM.cgf", "name":"HG00733-GS000016548-ASM", "id":98 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01565-GS000016975-ASM.cgf", "name":"HG01565-GS000016975-ASM", "id":99 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01566-GS000016976-ASM.cgf", "name":"HG01566-GS000016976-ASM", "id":100 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01567-GS000012114-ASM.cgf", "name":"HG01567-GS000012114-ASM", "id":101 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01571-GS000013212-ASM.cgf", "name":"HG01571-GS000013212-ASM", "id":102 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01572-GS000013211-ASM.cgf", "name":"HG01572-GS000013211-ASM", "id":103 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01573-GS000012117-ASM.cgf", "name":"HG01573-GS000012117-ASM", "id":104 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01577-GS000012118-ASM.cgf", "name":"HG01577-GS000012118-ASM", "id":105 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01578-GS000012119-ASM.cgf", "name":"HG01578-GS000012119-ASM", "id":106 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01579-GS000012120-ASM.cgf", "name":"HG01579-GS000012120-ASM", "id":107 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01892-GS000012121-ASM.cgf", "name":"HG01892-GS000012121-ASM", "id":108 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01893-GS000012579-ASM.cgf", "name":"HG01893-GS000012579-ASM", "id":109 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01898-GS000013210-ASM.cgf", "name":"HG01898-GS000013210-ASM", "id":110 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01917-GS000012124-ASM.cgf", "name":"HG01917-GS000012124-ASM", "id":111 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01918-GS000013461-ASM.cgf", "name":"HG01918-GS000013461-ASM", "id":112 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01919-GS000013410-ASM.cgf", "name":"HG01919-GS000013410-ASM", "id":113 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01920-GS000013208-ASM.cgf", "name":"HG01920-GS000013208-ASM", "id":114 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01921-GS000012128-ASM.cgf", "name":"HG01921-GS000012128-ASM", "id":115 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01923-GS000013459-ASM.cgf", "name":"HG01923-GS000013459-ASM", "id":116 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01924-GS000013460-ASM.cgf", "name":"HG01924-GS000013460-ASM", "id":117 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01925-GS000013413-ASM.cgf", "name":"HG01925-GS000013413-ASM", "id":118 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01926-GS000013203-ASM.cgf", "name":"HG01926-GS000013203-ASM", "id":119 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01927-GS000013202-ASM.cgf", "name":"HG01927-GS000013202-ASM", "id":120 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01928-GS000013201-ASM.cgf", "name":"HG01928-GS000013201-ASM", "id":121 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01932-GS000012136-ASM.cgf", "name":"HG01932-GS000012136-ASM", "id":122 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01933-GS000012137-ASM.cgf", "name":"HG01933-GS000012137-ASM", "id":123 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01934-GS000012138-ASM.cgf", "name":"HG01934-GS000012138-ASM", "id":124 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01935-GS000012139-ASM.cgf", "name":"HG01935-GS000012139-ASM", "id":125 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01936-GS000013433-ASM.cgf", "name":"HG01936-GS000013433-ASM", "id":126 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01937-GS000013199-ASM.cgf", "name":"HG01937-GS000013199-ASM", "id":127 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01938-GS000013425-ASM.cgf", "name":"HG01938-GS000013425-ASM", "id":128 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01939-GS000013197-ASM.cgf", "name":"HG01939-GS000013197-ASM", "id":129 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01940-GS000012144-ASM.cgf", "name":"HG01940-GS000012144-ASM", "id":130 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01941-GS000012145-ASM.cgf", "name":"HG01941-GS000012145-ASM", "id":131 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01942-GS000012146-ASM.cgf", "name":"HG01942-GS000012146-ASM", "id":132 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01943-GS000012147-ASM.cgf", "name":"HG01943-GS000012147-ASM", "id":133 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01944-GS000012148-ASM.cgf", "name":"HG01944-GS000012148-ASM", "id":134 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01945-GS000012149-ASM.cgf", "name":"HG01945-GS000012149-ASM", "id":135 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01946-GS000016667-ASM.cgf", "name":"HG01946-GS000016667-ASM", "id":136 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01950-GS000012150-ASM.cgf", "name":"HG01950-GS000012150-ASM", "id":137 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01951-GS000012151-ASM.cgf", "name":"HG01951-GS000012151-ASM", "id":138 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01952-GS000013196-ASM.cgf", "name":"HG01952-GS000013196-ASM", "id":139 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01953-GS000013195-ASM.cgf", "name":"HG01953-GS000013195-ASM", "id":140 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01954-GS000013458-ASM.cgf", "name":"HG01954-GS000013458-ASM", "id":141 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01955-GS000013457-ASM.cgf", "name":"HG01955-GS000013457-ASM", "id":142 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01967-GS000013456-ASM.cgf", "name":"HG01967-GS000013456-ASM", "id":143 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01968-GS000017246-ASM.cgf", "name":"HG01968-GS000017246-ASM", "id":144 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01969-GS000016666-ASM.cgf", "name":"HG01969-GS000016666-ASM", "id":145 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01970-GS000016665-ASM.cgf", "name":"HG01970-GS000016665-ASM", "id":146 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01971-GS000017156-ASM.cgf", "name":"HG01971-GS000017156-ASM", "id":147 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01972-GS000017132-ASM.cgf", "name":"HG01972-GS000017132-ASM", "id":148 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01973-GS000017130-ASM.cgf", "name":"HG01973-GS000017130-ASM", "id":149 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01974-GS000017158-ASM.cgf", "name":"HG01974-GS000017158-ASM", "id":150 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01975-GS000017271-ASM.cgf", "name":"HG01975-GS000017271-ASM", "id":151 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01976-GS000017160-ASM.cgf", "name":"HG01976-GS000017160-ASM", "id":152 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01977-GS000016364-ASM.cgf", "name":"HG01977-GS000016364-ASM", "id":153 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01978-GS000017124-ASM.cgf", "name":"HG01978-GS000017124-ASM", "id":154 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01979-GS000017123-ASM.cgf", "name":"HG01979-GS000017123-ASM", "id":155 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01980-GS000017122-ASM.cgf", "name":"HG01980-GS000017122-ASM", "id":156 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01981-GS000017103-ASM.cgf", "name":"HG01981-GS000017103-ASM", "id":157 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01991-GS000017121-ASM.cgf", "name":"HG01991-GS000017121-ASM", "id":158 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01992-GS000017118-ASM.cgf", "name":"HG01992-GS000017118-ASM", "id":159 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01993-GS000017104-ASM.cgf", "name":"HG01993-GS000017104-ASM", "id":160 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01997-GS000017117-ASM.cgf", "name":"HG01997-GS000017117-ASM", "id":161 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG01998-GS000017115-ASM.cgf", "name":"HG01998-GS000017115-ASM", "id":162 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02003-GS000017113-ASM.cgf", "name":"HG02003-GS000017113-ASM", "id":163 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02004-GS000017112-ASM.cgf", "name":"HG02004-GS000017112-ASM", "id":164 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02008-GS000017110-ASM.cgf", "name":"HG02008-GS000017110-ASM", "id":165 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02024-GS000012106-ASM.cgf", "name":"HG02024-GS000012106-ASM", "id":166 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02024-GS000017236-ASM.cgf", "name":"HG02024-GS000017236-ASM", "id":167 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02025-GS000012107-ASM.cgf", "name":"HG02025-GS000012107-ASM", "id":168 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02025-GS000017183-ASM.cgf", "name":"HG02025-GS000017183-ASM", "id":169 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02026-GS000017184-ASM.cgf", "name":"HG02026-GS000017184-ASM", "id":170 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02026-GS000017897-ASM.cgf", "name":"HG02026-GS000017897-ASM", "id":171 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02089-GS000017109-ASM.cgf", "name":"HG02089-GS000017109-ASM", "id":172 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02090-GS000017107-ASM.cgf", "name":"HG02090-GS000017107-ASM", "id":173 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02091-GS000016958-ASM.cgf", "name":"HG02091-GS000016958-ASM", "id":174 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02104-GS000016344-ASM.cgf", "name":"HG02104-GS000016344-ASM", "id":175 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02105-GS000016959-ASM.cgf", "name":"HG02105-GS000016959-ASM", "id":176 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02106-GS000016960-ASM.cgf", "name":"HG02106-GS000016960-ASM", "id":177 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02146-GS000016342-ASM.cgf", "name":"HG02146-GS000016342-ASM", "id":178 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02147-GS000016341-ASM.cgf", "name":"HG02147-GS000016341-ASM", "id":179 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02148-GS000016961-ASM.cgf", "name":"HG02148-GS000016961-ASM", "id":180 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02259-GS000016052-ASM.cgf", "name":"HG02259-GS000016052-ASM", "id":181 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02260-GS000016826-ASM.cgf", "name":"HG02260-GS000016826-ASM", "id":182 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02261-GS000016827-ASM.cgf", "name":"HG02261-GS000016827-ASM", "id":183 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02271-GS000017178-ASM.cgf", "name":"HG02271-GS000017178-ASM", "id":184 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02272-GS000017901-ASM.cgf", "name":"HG02272-GS000017901-ASM", "id":185 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02273-GS000017721-ASM.cgf", "name":"HG02273-GS000017721-ASM", "id":186 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02277-GS000016049-ASM.cgf", "name":"HG02277-GS000016049-ASM", "id":187 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02278-GS000017134-ASM.cgf", "name":"HG02278-GS000017134-ASM", "id":188 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02279-GS000017135-ASM.cgf", "name":"HG02279-GS000017135-ASM", "id":189 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02285-GS000016420-ASM.cgf", "name":"HG02285-GS000016420-ASM", "id":190 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02286-GS000016419-ASM.cgf", "name":"HG02286-GS000016419-ASM", "id":191 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02287-GS000017903-ASM.cgf", "name":"HG02287-GS000017903-ASM", "id":192 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02291-GS000017105-ASM.cgf", "name":"HG02291-GS000017105-ASM", "id":193 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02292-GS000013194-ASM.cgf", "name":"HG02292-GS000013194-ASM", "id":194 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02293-GS000013193-ASM.cgf", "name":"HG02293-GS000013193-ASM", "id":195 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02301-GS000017904-ASM.cgf", "name":"HG02301-GS000017904-ASM", "id":196 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02302-GS000016554-ASM.cgf", "name":"HG02302-GS000016554-ASM", "id":197 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02303-GS000016553-ASM.cgf", "name":"HG02303-GS000016553-ASM", "id":198 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02490-GS000013415-ASM.cgf", "name":"HG02490-GS000013415-ASM", "id":199 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02491-GS000013191-ASM.cgf", "name":"HG02491-GS000013191-ASM", "id":200 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02492-GS000013190-ASM.cgf", "name":"HG02492-GS000013190-ASM", "id":201 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02600-GS000016696-ASM.cgf", "name":"HG02600-GS000016696-ASM", "id":202 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02601-GS000016693-ASM.cgf", "name":"HG02601-GS000016693-ASM", "id":203 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02602-GS000016692-ASM.cgf", "name":"HG02602-GS000016692-ASM", "id":204 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02603-GS000016691-ASM.cgf", "name":"HG02603-GS000016691-ASM", "id":205 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02604-GS000016690-ASM.cgf", "name":"HG02604-GS000016690-ASM", "id":206 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02605-GS000016689-ASM.cgf", "name":"HG02605-GS000016689-ASM", "id":207 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02654-GS000016688-ASM.cgf", "name":"HG02654-GS000016688-ASM", "id":208 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02655-GS000016687-ASM.cgf", "name":"HG02655-GS000016687-ASM", "id":209 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02656-GS000016686-ASM.cgf", "name":"HG02656-GS000016686-ASM", "id":210 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02657-GS000016544-ASM.cgf", "name":"HG02657-GS000016544-ASM", "id":211 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02658-GS000017179-ASM.cgf", "name":"HG02658-GS000017179-ASM", "id":212 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02659-GS000017180-ASM.cgf", "name":"HG02659-GS000017180-ASM", "id":213 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02660-GS000017181-ASM.cgf", "name":"HG02660-GS000017181-ASM", "id":214 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02661-GS000017902-ASM.cgf", "name":"HG02661-GS000017902-ASM", "id":215 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02662-GS000017126-ASM.cgf", "name":"HG02662-GS000017126-ASM", "id":216 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02684-GS000017127-ASM.cgf", "name":"HG02684-GS000017127-ASM", "id":217 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02685-GS000017225-ASM.cgf", "name":"HG02685-GS000017225-ASM", "id":218 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02686-GS000016439-ASM.cgf", "name":"HG02686-GS000016439-ASM", "id":219 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02687-GS000016685-ASM.cgf", "name":"HG02687-GS000016685-ASM", "id":220 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02688-GS000017228-ASM.cgf", "name":"HG02688-GS000017228-ASM", "id":221 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02689-GS000017128-ASM.cgf", "name":"HG02689-GS000017128-ASM", "id":222 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02696-GS000016436-ASM.cgf", "name":"HG02696-GS000016436-ASM", "id":223 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02697-GS000017229-ASM.cgf", "name":"HG02697-GS000017229-ASM", "id":224 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02698-GS000017230-ASM.cgf", "name":"HG02698-GS000017230-ASM", "id":225 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02724-GS000016990-ASM.cgf", "name":"HG02724-GS000016990-ASM", "id":226 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02725-GS000016997-ASM.cgf", "name":"HG02725-GS000016997-ASM", "id":227 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02726-GS000016996-ASM.cgf", "name":"HG02726-GS000016996-ASM", "id":228 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02727-GS000016995-ASM.cgf", "name":"HG02727-GS000016995-ASM", "id":229 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02728-GS000017019-ASM.cgf", "name":"HG02728-GS000017019-ASM", "id":230 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02729-GS000017017-ASM.cgf", "name":"HG02729-GS000017017-ASM", "id":231 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02733-GS000017232-ASM.cgf", "name":"HG02733-GS000017232-ASM", "id":232 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02734-GS000017233-ASM.cgf", "name":"HG02734-GS000017233-ASM", "id":233 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02735-GS000016431-ASM.cgf", "name":"HG02735-GS000016431-ASM", "id":234 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02783-GS000017016-ASM.cgf", "name":"HG02783-GS000017016-ASM", "id":235 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02784-GS000017015-ASM.cgf", "name":"HG02784-GS000017015-ASM", "id":236 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02785-GS000017012-ASM.cgf", "name":"HG02785-GS000017012-ASM", "id":237 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02786-GS000017013-ASM.cgf", "name":"HG02786-GS000017013-ASM", "id":238 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02787-GS000017021-ASM.cgf", "name":"HG02787-GS000017021-ASM", "id":239 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02789-GS000017011-ASM.cgf", "name":"HG02789-GS000017011-ASM", "id":240 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02790-GS000017018-ASM.cgf", "name":"HG02790-GS000017018-ASM", "id":241 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG02791-GS000017014-ASM.cgf", "name":"HG02791-GS000017014-ASM", "id":242 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG03237-GS000017140-ASM.cgf", "name":"HG03237-GS000017140-ASM", "id":243 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG03238-GS000017139-ASM.cgf", "name":"HG03238-GS000017139-ASM", "id":244 });
  cgf_info.cgf.push({ "file":"/data/cgf/HG03239-GS000017138-ASM.cgf", "name":"HG03239-GS000017138-ASM", "id":245 });
  cgf_info.cgf.push({ "file":"/data/cgf/hg19.cgf", "name":"hg19", "id":246 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu011C57-GS01669-DNA_B05.cgf", "name":"hu011C57-GS01669-DNA_B05", "id":247 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu016B28-GS01669-DNA_H03.cgf", "name":"hu016B28-GS01669-DNA_H03", "id":248 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu01F73B-GS03133-DNA_A02.cgf", "name":"hu01F73B-GS03133-DNA_A02", "id":249 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu0211D6-GS01175-DNA_E02.cgf", "name":"hu0211D6-GS01175-DNA_E02", "id":250 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu025CEA-GS01669-DNA_D02.cgf", "name":"hu025CEA-GS01669-DNA_D02", "id":251 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu032C04-GS01669-DNA_A10.cgf", "name":"hu032C04-GS01669-DNA_A10", "id":252 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu034DB1-GS00253-DNA_A02.cgf", "name":"hu034DB1-GS00253-DNA_A02", "id":253 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu034DB1-GS01669-DNA_A03.cgf", "name":"hu034DB1-GS01669-DNA_A03", "id":254 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu03E3D2-GS03132-DNA_D01.cgf", "name":"hu03E3D2-GS03132-DNA_D01", "id":255 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu040C0A-GS01175-DNA_F05.cgf", "name":"hu040C0A-GS01175-DNA_F05", "id":256 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu0486D6-GS03132-DNA_C01.cgf", "name":"hu0486D6-GS03132-DNA_C01", "id":257 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu04DF3C-GS01670-DNA_A02.cgf", "name":"hu04DF3C-GS01670-DNA_A02", "id":258 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu04F220-GS01670-DNA_A01.cgf", "name":"hu04F220-GS01670-DNA_A01", "id":259 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu04FD18-GS00253-DNA_F01.cgf", "name":"hu04FD18-GS00253-DNA_F01", "id":260 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu050E9C-GS01173-DNA_G06.cgf", "name":"hu050E9C-GS01173-DNA_G06", "id":261 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu05FD49-GS01175-DNA_D04.cgf", "name":"hu05FD49-GS01175-DNA_D04", "id":262 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu085B6D-GS03132-DNA_A01.cgf", "name":"hu085B6D-GS03132-DNA_A01", "id":263 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu089792-GS02269-DNA_B02.cgf", "name":"hu089792-GS02269-DNA_B02", "id":264 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu0A4518-GS01670-DNA_F01.cgf", "name":"hu0A4518-GS01670-DNA_F01", "id":265 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu0CF2EE-GS01175-DNA_F06.cgf", "name":"hu0CF2EE-GS01175-DNA_F06", "id":266 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu0D1FA1-GS01669-DNA_G09.cgf", "name":"hu0D1FA1-GS01669-DNA_G09", "id":267 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu0D879F-GS00253-DNA_G01.cgf", "name":"hu0D879F-GS00253-DNA_G01", "id":268 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu0D879F-GS01669-DNA_A08.cgf", "name":"hu0D879F-GS01669-DNA_A08", "id":269 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu0E64A1-GS01173-DNA_B02.cgf", "name":"hu0E64A1-GS01173-DNA_B02", "id":270 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu0E7AAF-GS03023-DNA_A01.cgf", "name":"hu0E7AAF-GS03023-DNA_A01", "id":271 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu1187FF-GS02269-DNA_A04.cgf", "name":"hu1187FF-GS02269-DNA_A04", "id":272 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu132B5C-GS01670-DNA_E01.cgf", "name":"hu132B5C-GS01670-DNA_E01", "id":273 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu1378E3-GS01669-DNA_C03.cgf", "name":"hu1378E3-GS01669-DNA_C03", "id":274 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu15FECA-GS01175-DNA_F03.cgf", "name":"hu15FECA-GS01175-DNA_F03", "id":275 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu1904EC-GS01175-DNA_E06.cgf", "name":"hu1904EC-GS01175-DNA_E06", "id":276 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu19C09F-GS01669-DNA_F05.cgf", "name":"hu19C09F-GS01669-DNA_F05", "id":277 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu1E868D-GS03166-DNA_C02.cgf", "name":"hu1E868D-GS03166-DNA_C02", "id":278 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu1F73AB-GS01669-DNA_B07.cgf", "name":"hu1F73AB-GS01669-DNA_B07", "id":279 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu241DEA-GS01175-DNA_D05.cgf", "name":"hu241DEA-GS01175-DNA_D05", "id":280 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu24A473-GS03132-DNA_F01.cgf", "name":"hu24A473-GS03132-DNA_F01", "id":281 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu24C863-GS01670-DNA_B02.cgf", "name":"hu24C863-GS01670-DNA_B02", "id":282 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu259AC7-GS01175-DNA_A05.cgf", "name":"hu259AC7-GS01175-DNA_A05", "id":283 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu26B551-GS01175-DNA_F04.cgf", "name":"hu26B551-GS01175-DNA_F04", "id":284 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu27FD1F-GS02269-DNA_B04.cgf", "name":"hu27FD1F-GS02269-DNA_B04", "id":285 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu2843C9-GS01669-DNA_D05.cgf", "name":"hu2843C9-GS01669-DNA_D05", "id":286 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu297562-GS01669-DNA_H01.cgf", "name":"hu297562-GS01669-DNA_H01", "id":287 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu2C1D94-GS01175-DNA_H02.cgf", "name":"hu2C1D94-GS01175-DNA_H02", "id":288 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu2D6140-GS01173-DNA_F06.cgf", "name":"hu2D6140-GS01173-DNA_F06", "id":289 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu2DBF2D-GS01173-DNA_G02.cgf", "name":"hu2DBF2D-GS01173-DNA_G02", "id":290 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu2FEC01-GS01175-DNA_E03.cgf", "name":"hu2FEC01-GS01175-DNA_E03", "id":291 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu3073E3-GS01239-DNA_D02.cgf", "name":"hu3073E3-GS01239-DNA_D02", "id":292 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu33E2D9-GS01669-DNA_G10.cgf", "name":"hu33E2D9-GS01669-DNA_G10", "id":293 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu342A08-GS01175-DNA_B05.cgf", "name":"hu342A08-GS01175-DNA_B05", "id":294 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu34D5B9-GS01173-DNA_C07.cgf", "name":"hu34D5B9-GS01173-DNA_C07", "id":295 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu34D5B9-GS01670-DNA_E02.cgf", "name":"hu34D5B9-GS01670-DNA_E02", "id":296 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu38168C-GS01173-DNA_H06.cgf", "name":"hu38168C-GS01173-DNA_H06", "id":297 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu3A1B15-GS02269-DNA_C01.cgf", "name":"hu3A1B15-GS02269-DNA_C01", "id":298 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu3A8D13-GS01669-DNA_E11.cgf", "name":"hu3A8D13-GS01669-DNA_E11", "id":299 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu3C0611-GS01669-DNA_E08.cgf", "name":"hu3C0611-GS01669-DNA_E08", "id":300 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu3CAB43-GS01175-DNA_D03.cgf", "name":"hu3CAB43-GS01175-DNA_D03", "id":301 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu3DC5EA-GS03184-DNA_F01.cgf", "name":"hu3DC5EA-GS03184-DNA_F01", "id":302 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu3F864B-GS01669-DNA_F08.cgf", "name":"hu3F864B-GS01669-DNA_F08", "id":303 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu4040B8-GS01175-DNA_D01.cgf", "name":"hu4040B8-GS01175-DNA_D01", "id":304 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu42B622-GS03387-DNA_A03.cgf", "name":"hu42B622-GS03387-DNA_A03", "id":305 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu42D651-GS01669-DNA_G03.cgf", "name":"hu42D651-GS01669-DNA_G03", "id":306 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu432EB5-GS01670-DNA_F02.cgf", "name":"hu432EB5-GS01670-DNA_F02", "id":307 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu4339C0-GS01175-DNA_H01.cgf", "name":"hu4339C0-GS01175-DNA_H01", "id":308 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu43860C-GS00253-DNA_A01.cgf", "name":"hu43860C-GS00253-DNA_A01", "id":309 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu448C4B-GS01669-DNA_E07.cgf", "name":"hu448C4B-GS01669-DNA_E07", "id":310 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu44DCFF-GS01669-DNA_C07.cgf", "name":"hu44DCFF-GS01669-DNA_C07", "id":311 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu470099-GS01175-DNA_G06.cgf", "name":"hu470099-GS01175-DNA_G06", "id":312 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu474789-GS02269-DNA_G04.cgf", "name":"hu474789-GS02269-DNA_G04", "id":313 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu48C4EB-GS01239-DNA_D01.cgf", "name":"hu48C4EB-GS01239-DNA_D01", "id":314 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu49F623-GS02269-DNA_F01.cgf", "name":"hu49F623-GS02269-DNA_F01", "id":315 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu4B0812-GS01669-DNA_A11.cgf", "name":"hu4B0812-GS01669-DNA_A11", "id":316 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu4BE6F2-GS01669-DNA_A06.cgf", "name":"hu4BE6F2-GS01669-DNA_A06", "id":317 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu4BF398-GS01175-DNA_A06.cgf", "name":"hu4BF398-GS01175-DNA_A06", "id":318 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu4CA5B9-GS01669-DNA_B03.cgf", "name":"hu4CA5B9-GS01669-DNA_B03", "id":319 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu4FE0D1-GS01669-DNA_G04.cgf", "name":"hu4FE0D1-GS01669-DNA_G04", "id":320 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu52B7E5-GS01669-DNA_H04.cgf", "name":"hu52B7E5-GS01669-DNA_H04", "id":321 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu52F345-GS01669-DNA_G07.cgf", "name":"hu52F345-GS01669-DNA_G07", "id":322 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu553620-GS01669-DNA_B08.cgf", "name":"hu553620-GS01669-DNA_B08", "id":323 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu566AA7-GS03274-DNA_H01.cgf", "name":"hu566AA7-GS03274-DNA_H01", "id":324 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu57A769-GS01669-DNA_C08.cgf", "name":"hu57A769-GS01669-DNA_C08", "id":325 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu589D0B-GS01669-DNA_H11.cgf", "name":"hu589D0B-GS01669-DNA_H11", "id":326 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu5962F5-GS03166-DNA_C01.cgf", "name":"hu5962F5-GS03166-DNA_C01", "id":327 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu599905-GS01670-DNA_H01.cgf", "name":"hu599905-GS01670-DNA_H01", "id":328 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu5B8771-GS02269-DNA_B01.cgf", "name":"hu5B8771-GS02269-DNA_B01", "id":329 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu5CD2C6-GS01669-DNA_F04.cgf", "name":"hu5CD2C6-GS01669-DNA_F04", "id":330 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu5E55F5-GS02269-DNA_B03.cgf", "name":"hu5E55F5-GS02269-DNA_B03", "id":331 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu5FA322-GS01669-DNA_E06.cgf", "name":"hu5FA322-GS01669-DNA_E06", "id":332 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu5FB1B9-GS01669-DNA_C06.cgf", "name":"hu5FB1B9-GS01669-DNA_C06", "id":333 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu5FCE15-GS01195-DNA_B01.cgf", "name":"hu5FCE15-GS01195-DNA_B01", "id":334 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu60180F-GS01175-DNA_B06.cgf", "name":"hu60180F-GS01175-DNA_B06", "id":335 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu602487-GS02269-DNA_F02.cgf", "name":"hu602487-GS02269-DNA_F02", "id":336 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu604D39-GS00253-DNA_B02.cgf", "name":"hu604D39-GS00253-DNA_B02", "id":337 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu60AB7C-GS01670-DNA_D01.cgf", "name":"hu60AB7C-GS01670-DNA_D01", "id":338 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu619F51-GS01669-DNA_B02.cgf", "name":"hu619F51-GS01669-DNA_B02", "id":339 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu620F18-GS02269-DNA_E02.cgf", "name":"hu620F18-GS02269-DNA_E02", "id":340 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu627574-GS01669-DNA_E01.cgf", "name":"hu627574-GS01669-DNA_E01", "id":341 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu63EB0A-GS01175-DNA_C05.cgf", "name":"hu63EB0A-GS01175-DNA_C05", "id":342 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu64DBF7-GS01669-DNA_D03.cgf", "name":"hu64DBF7-GS01669-DNA_D03", "id":343 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu661AD0-GS01670-DNA_C02.cgf", "name":"hu661AD0-GS01670-DNA_C02", "id":344 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu67EBB3-GS01669-DNA_G06.cgf", "name":"hu67EBB3-GS01669-DNA_G06", "id":345 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu687B6B-GS01669-DNA_H07.cgf", "name":"hu687B6B-GS01669-DNA_H07", "id":346 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu68929D-GS01669-DNA_F11.cgf", "name":"hu68929D-GS01669-DNA_F11", "id":347 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu6A01AF-GS03132-DNA_G01.cgf", "name":"hu6A01AF-GS03132-DNA_G01", "id":348 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu6C733E-GS01669-DNA_E05.cgf", "name":"hu6C733E-GS01669-DNA_E05", "id":349 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu6E4515-GS01173-DNA_D07.cgf", "name":"hu6E4515-GS01173-DNA_D07", "id":350 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu6EDC7E-GS02995-DNA_F02.cgf", "name":"hu6EDC7E-GS02995-DNA_F02", "id":351 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu704A85-GS01239-DNA_E01.cgf", "name":"hu704A85-GS01239-DNA_E01", "id":352 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu7123C1-GS01669-DNA_D07.cgf", "name":"hu7123C1-GS01669-DNA_D07", "id":353 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu71E59D-GS01239-DNA_F01.cgf", "name":"hu71E59D-GS01239-DNA_F01", "id":354 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu72A81D-GS01173-DNA_C02.cgf", "name":"hu72A81D-GS01173-DNA_C02", "id":355 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu72C17A-GS01669-DNA_F07.cgf", "name":"hu72C17A-GS01669-DNA_F07", "id":356 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu7379BC-GS03052-DNA_E01.cgf", "name":"hu7379BC-GS03052-DNA_E01", "id":357 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu76CAA5-GS02269-DNA_G03.cgf", "name":"hu76CAA5-GS02269-DNA_G03", "id":358 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu775356-GS01175-DNA_A07.cgf", "name":"hu775356-GS01175-DNA_A07", "id":359 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu7852C5-GS01175-DNA_H05.cgf", "name":"hu7852C5-GS01175-DNA_H05", "id":360 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu79F922-GS01669-DNA_D06.cgf", "name":"hu79F922-GS01669-DNA_D06", "id":361 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu7A2F1D-GS01669-DNA_C01.cgf", "name":"hu7A2F1D-GS01669-DNA_C01", "id":362 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu7A4AD1-GS01669-DNA_C05.cgf", "name":"hu7A4AD1-GS01669-DNA_C05", "id":363 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu7B594C-GS01669-DNA_G05.cgf", "name":"hu7B594C-GS01669-DNA_G05", "id":364 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu7C3A81-GS03052-DNA_F01.cgf", "name":"hu7C3A81-GS03052-DNA_F01", "id":365 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu7DCBF9-GS01175-DNA_A03.cgf", "name":"hu7DCBF9-GS01175-DNA_A03", "id":366 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu8073B9-GS01239-DNA_G01.cgf", "name":"hu8073B9-GS01239-DNA_G01", "id":367 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu80AD0F-GS03166-DNA_B03.cgf", "name":"hu80AD0F-GS03166-DNA_B03", "id":368 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu8229AE-GS01173-DNA_A07.cgf", "name":"hu8229AE-GS01173-DNA_A07", "id":369 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu82436A-GS02269-DNA_F03.cgf", "name":"hu82436A-GS02269-DNA_F03", "id":370 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu826751-GS03052-DNA_B01.cgf", "name":"hu826751-GS03052-DNA_B01", "id":371 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu82E689-GS01669-DNA_A09.cgf", "name":"hu82E689-GS01669-DNA_A09", "id":372 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu83BC6A-GS03023-DNA_C01.cgf", "name":"hu83BC6A-GS03023-DNA_C01", "id":373 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu868880-GS02269-DNA_E01.cgf", "name":"hu868880-GS02269-DNA_E01", "id":374 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu88A079-GS01669-DNA_F03.cgf", "name":"hu88A079-GS01669-DNA_F03", "id":375 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu8A5FBF-GS03184-DNA_A02.cgf", "name":"hu8A5FBF-GS03184-DNA_A02", "id":376 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu8E2A35-GS01669-DNA_G11.cgf", "name":"hu8E2A35-GS01669-DNA_G11", "id":377 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu8E87A9-GS01669-DNA_B09.cgf", "name":"hu8E87A9-GS01669-DNA_B09", "id":378 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu8F918A-GS01670-DNA_B01.cgf", "name":"hu8F918A-GS01670-DNA_B01", "id":379 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu90B053-GS01670-DNA_D02.cgf", "name":"hu90B053-GS01670-DNA_D02", "id":380 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu925B56-GS03274-DNA_B01.cgf", "name":"hu925B56-GS03274-DNA_B01", "id":381 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu92C40A-GS01175-DNA_G03.cgf", "name":"hu92C40A-GS01175-DNA_G03", "id":382 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu92FD55-GS01669-DNA_A04.cgf", "name":"hu92FD55-GS01669-DNA_A04", "id":383 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu9385BA-GS00253-DNA_E01.cgf", "name":"hu9385BA-GS00253-DNA_E01", "id":384 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu939B7C-GS01670-DNA_C01.cgf", "name":"hu939B7C-GS01670-DNA_C01", "id":385 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu955EE1-GS01173-DNA_G01.cgf", "name":"hu955EE1-GS01173-DNA_G01", "id":386 });
  cgf_info.cgf.push({ "file":"/data/cgf/hu9E6329-GS03052-DNA_A01.cgf", "name":"hu9E6329-GS03052-DNA_A01", "id":387 });
  cgf_info.cgf.push({ "file":"/data/cgf/huA02824-GS02269-DNA_G01.cgf", "name":"huA02824-GS02269-DNA_G01", "id":388 });
  cgf_info.cgf.push({ "file":"/data/cgf/huA05317-GS01669-DNA_E10.cgf", "name":"huA05317-GS01669-DNA_E10", "id":389 });
  cgf_info.cgf.push({ "file":"/data/cgf/huA0E089-GS01175-DNA_B04.cgf", "name":"huA0E089-GS01175-DNA_B04", "id":390 });
  cgf_info.cgf.push({ "file":"/data/cgf/huA3A02C-GS03166-DNA_E02.cgf", "name":"huA3A02C-GS03166-DNA_E02", "id":391 });
  cgf_info.cgf.push({ "file":"/data/cgf/huA49E22-GS01669-DNA_E04.cgf", "name":"huA49E22-GS01669-DNA_E04", "id":392 });
  cgf_info.cgf.push({ "file":"/data/cgf/huA4E2CF-GS01173-DNA_D04.cgf", "name":"huA4E2CF-GS01173-DNA_D04", "id":393 });
  cgf_info.cgf.push({ "file":"/data/cgf/huA4F281-GS01669-DNA_H10.cgf", "name":"huA4F281-GS01669-DNA_H10", "id":394 });
  cgf_info.cgf.push({ "file":"/data/cgf/huA5FD8B-GS02269-DNA_C02.cgf", "name":"huA5FD8B-GS02269-DNA_C02", "id":395 });
  cgf_info.cgf.push({ "file":"/data/cgf/huAA245C-GS02269-DNA_D03.cgf", "name":"huAA245C-GS02269-DNA_D03", "id":396 });
  cgf_info.cgf.push({ "file":"/data/cgf/huAA53E0-GS02260-DNA_A01.cgf", "name":"huAA53E0-GS02260-DNA_A01", "id":397 });
  cgf_info.cgf.push({ "file":"/data/cgf/huAE4A11-GS01669-DNA_F02.cgf", "name":"huAE4A11-GS01669-DNA_F02", "id":398 });
  cgf_info.cgf.push({ "file":"/data/cgf/huAE6220-GS00253-DNA_H01.cgf", "name":"huAE6220-GS00253-DNA_H01", "id":399 });
  cgf_info.cgf.push({ "file":"/data/cgf/huAEADC0-GS01175-DNA_E05.cgf", "name":"huAEADC0-GS01175-DNA_E05", "id":400 });
  cgf_info.cgf.push({ "file":"/data/cgf/huAEC1B0-GS01175-DNA_C04.cgf", "name":"huAEC1B0-GS01175-DNA_C04", "id":401 });
  cgf_info.cgf.push({ "file":"/data/cgf/huAFA81C-GS01669-DNA_E03.cgf", "name":"huAFA81C-GS01669-DNA_E03", "id":402 });
  cgf_info.cgf.push({ "file":"/data/cgf/huB1488D-GS03052-DNA_H01.cgf", "name":"huB1488D-GS03052-DNA_H01", "id":403 });
  cgf_info.cgf.push({ "file":"/data/cgf/huB1FD55-GS01173-DNA_B07.cgf", "name":"huB1FD55-GS01173-DNA_B07", "id":404 });
  cgf_info.cgf.push({ "file":"/data/cgf/huB4883B-GS02269-DNA_H04.cgf", "name":"huB4883B-GS02269-DNA_H04", "id":405 });
  cgf_info.cgf.push({ "file":"/data/cgf/huB4940E-GS01175-DNA_C02.cgf", "name":"huB4940E-GS01175-DNA_C02", "id":406 });
  cgf_info.cgf.push({ "file":"/data/cgf/huB4D223-GS01669-DNA_H06.cgf", "name":"huB4D223-GS01669-DNA_H06", "id":407 });
  cgf_info.cgf.push({ "file":"/data/cgf/huB4F9B2-GS02269-DNA_A05.cgf", "name":"huB4F9B2-GS02269-DNA_A05", "id":408 });
  cgf_info.cgf.push({ "file":"/data/cgf/huBA30D4-GS01173-DNA_H05.cgf", "name":"huBA30D4-GS01173-DNA_H05", "id":409 });
  cgf_info.cgf.push({ "file":"/data/cgf/huBAAC98-GS01173-DNA_F02.cgf", "name":"huBAAC98-GS01173-DNA_F02", "id":410 });
  cgf_info.cgf.push({ "file":"/data/cgf/huBE0B25-GS01669-DNA_D01.cgf", "name":"huBE0B25-GS01669-DNA_D01", "id":411 });
  cgf_info.cgf.push({ "file":"/data/cgf/huBEDA0B-GS00253-DNA_C01.cgf", "name":"huBEDA0B-GS00253-DNA_C01", "id":412 });
  cgf_info.cgf.push({ "file":"/data/cgf/huBEDA0B-GS01669-DNA_E02.cgf", "name":"huBEDA0B-GS01669-DNA_E02", "id":413 });
  cgf_info.cgf.push({ "file":"/data/cgf/huC14AE1-GS01175-DNA_G05.cgf", "name":"huC14AE1-GS01175-DNA_G05", "id":414 });
  cgf_info.cgf.push({ "file":"/data/cgf/huC29627-GS01173-DNA_C06.cgf", "name":"huC29627-GS01173-DNA_C06", "id":415 });
  cgf_info.cgf.push({ "file":"/data/cgf/huC30901-GS00253-DNA_B01.cgf", "name":"huC30901-GS00253-DNA_B01", "id":416 });
  cgf_info.cgf.push({ "file":"/data/cgf/huC3160A-GS01669-DNA_G08.cgf", "name":"huC3160A-GS01669-DNA_G08", "id":417 });
  cgf_info.cgf.push({ "file":"/data/cgf/huC434ED-GS01669-DNA_D08.cgf", "name":"huC434ED-GS01669-DNA_D08", "id":418 });
  cgf_info.cgf.push({ "file":"/data/cgf/huC5733C-GS01669-DNA_F06.cgf", "name":"huC5733C-GS01669-DNA_F06", "id":419 });
  cgf_info.cgf.push({ "file":"/data/cgf/huC92BC9-GS01669-DNA_C11.cgf", "name":"huC92BC9-GS01669-DNA_C11", "id":420 });
  cgf_info.cgf.push({ "file":"/data/cgf/huC93106-GS01669-DNA_B11.cgf", "name":"huC93106-GS01669-DNA_B11", "id":421 });
  cgf_info.cgf.push({ "file":"/data/cgf/huCA017E-GS01175-DNA_B01.cgf", "name":"huCA017E-GS01175-DNA_B01", "id":422 });
  cgf_info.cgf.push({ "file":"/data/cgf/huCA14D2-GS01669-DNA_A07.cgf", "name":"huCA14D2-GS01669-DNA_A07", "id":423 });
  cgf_info.cgf.push({ "file":"/data/cgf/huCBDC6D-GS03166-DNA_A03.cgf", "name":"huCBDC6D-GS03166-DNA_A03", "id":424 });
  cgf_info.cgf.push({ "file":"/data/cgf/huCCAFD0-GS01669-DNA_B10.cgf", "name":"huCCAFD0-GS01669-DNA_B10", "id":425 });
  cgf_info.cgf.push({ "file":"/data/cgf/huCD380F-GS01175-DNA_H06.cgf", "name":"huCD380F-GS01175-DNA_H06", "id":426 });
  cgf_info.cgf.push({ "file":"/data/cgf/huD09534-GS01669-DNA_F10.cgf", "name":"huD09534-GS01669-DNA_F10", "id":427 });
  cgf_info.cgf.push({ "file":"/data/cgf/huD103CC-GS01669-DNA_C09.cgf", "name":"huD103CC-GS01669-DNA_C09", "id":428 });
  cgf_info.cgf.push({ "file":"/data/cgf/huD10E53-GS01669-DNA_B04.cgf", "name":"huD10E53-GS01669-DNA_B04", "id":429 });
  cgf_info.cgf.push({ "file":"/data/cgf/huD2B804-GS03132-DNA_B01.cgf", "name":"huD2B804-GS03132-DNA_B01", "id":430 });
  cgf_info.cgf.push({ "file":"/data/cgf/huD37D14-GS01175-DNA_A04.cgf", "name":"huD37D14-GS01175-DNA_A04", "id":431 });
  cgf_info.cgf.push({ "file":"/data/cgf/huD3A569-GS02269-DNA_F04.cgf", "name":"huD3A569-GS02269-DNA_F04", "id":432 });
  cgf_info.cgf.push({ "file":"/data/cgf/huD52556-GS01669-DNA_A02.cgf", "name":"huD52556-GS01669-DNA_A02", "id":433 });
  cgf_info.cgf.push({ "file":"/data/cgf/huD649F1-GS03133-DNA_D02.cgf", "name":"huD649F1-GS03133-DNA_D02", "id":434 });
  cgf_info.cgf.push({ "file":"/data/cgf/huD81F3D-GS01173-DNA_D06.cgf", "name":"huD81F3D-GS01173-DNA_D06", "id":435 });
  cgf_info.cgf.push({ "file":"/data/cgf/huD9EE1E-GS01669-DNA_F09.cgf", "name":"huD9EE1E-GS01669-DNA_F09", "id":436 });
  cgf_info.cgf.push({ "file":"/data/cgf/huDBD591-GS01175-DNA_A02.cgf", "name":"huDBD591-GS01175-DNA_A02", "id":437 });
  cgf_info.cgf.push({ "file":"/data/cgf/huDBF9DD-GS01669-DNA_E09.cgf", "name":"huDBF9DD-GS01669-DNA_E09", "id":438 });
  cgf_info.cgf.push({ "file":"/data/cgf/huDE435D-GS01669-DNA_D11.cgf", "name":"huDE435D-GS01669-DNA_D11", "id":439 });
  cgf_info.cgf.push({ "file":"/data/cgf/huDF04CC-GS01175-DNA_B03.cgf", "name":"huDF04CC-GS01175-DNA_B03", "id":440 });
  cgf_info.cgf.push({ "file":"/data/cgf/huE2E371-GS02269-DNA_H03.cgf", "name":"huE2E371-GS02269-DNA_H03", "id":441 });
  cgf_info.cgf.push({ "file":"/data/cgf/huE58004-GS01669-DNA_H02.cgf", "name":"huE58004-GS01669-DNA_H02", "id":442 });
  cgf_info.cgf.push({ "file":"/data/cgf/huE80E3D-GS00253-DNA_D01.cgf", "name":"huE80E3D-GS00253-DNA_D01", "id":443 });
  cgf_info.cgf.push({ "file":"/data/cgf/huE9B698-GS01669-DNA_H09.cgf", "name":"huE9B698-GS01669-DNA_H09", "id":444 });
  cgf_info.cgf.push({ "file":"/data/cgf/huEA4EE5-GS01669-DNA_G02.cgf", "name":"huEA4EE5-GS01669-DNA_G02", "id":445 });
  cgf_info.cgf.push({ "file":"/data/cgf/huEBD467-GS01670-DNA_G01.cgf", "name":"huEBD467-GS01670-DNA_G01", "id":446 });
  cgf_info.cgf.push({ "file":"/data/cgf/huEC6EEC-GS01175-DNA_H04.cgf", "name":"huEC6EEC-GS01175-DNA_H04", "id":447 });
  cgf_info.cgf.push({ "file":"/data/cgf/huED0F40-GS01669-DNA_H08.cgf", "name":"huED0F40-GS01669-DNA_H08", "id":448 });
  cgf_info.cgf.push({ "file":"/data/cgf/huEDEA65-GS01669-DNA_F01.cgf", "name":"huEDEA65-GS01669-DNA_F01", "id":449 });
  cgf_info.cgf.push({ "file":"/data/cgf/huEDF7DA-GS01669-DNA_C04.cgf", "name":"huEDF7DA-GS01669-DNA_C04", "id":450 });
  cgf_info.cgf.push({ "file":"/data/cgf/huF160AA-GS03166-DNA_G01.cgf", "name":"huF160AA-GS03166-DNA_G01", "id":451 });
  cgf_info.cgf.push({ "file":"/data/cgf/huF1DC30-GS01669-DNA_G01.cgf", "name":"huF1DC30-GS01669-DNA_G01", "id":452 });
  cgf_info.cgf.push({ "file":"/data/cgf/huF2DA6F-GS02269-DNA_A01.cgf", "name":"huF2DA6F-GS02269-DNA_A01", "id":453 });
  cgf_info.cgf.push({ "file":"/data/cgf/huF5AD12-GS01175-DNA_D06.cgf", "name":"huF5AD12-GS01175-DNA_D06", "id":454 });
  cgf_info.cgf.push({ "file":"/data/cgf/huF5E0B6-GS01175-DNA_G04.cgf", "name":"huF5E0B6-GS01175-DNA_G04", "id":455 });
  cgf_info.cgf.push({ "file":"/data/cgf/huF80F84-GS01669-DNA_B01.cgf", "name":"huF80F84-GS01669-DNA_B01", "id":456 });
  cgf_info.cgf.push({ "file":"/data/cgf/huF83462-GS03166-DNA_B02.cgf", "name":"huF83462-GS03166-DNA_B02", "id":457 });
  cgf_info.cgf.push({ "file":"/data/cgf/huFA70A3-GS01670-DNA_G02.cgf", "name":"huFA70A3-GS01670-DNA_G02", "id":458 });
  cgf_info.cgf.push({ "file":"/data/cgf/huFAF983-GS01175-DNA_F02.cgf", "name":"huFAF983-GS01175-DNA_F02", "id":459 });
  cgf_info.cgf.push({ "file":"/data/cgf/huFCC1C1-GS02269-DNA_A02.cgf", "name":"huFCC1C1-GS02269-DNA_A02", "id":460 });
  cgf_info.cgf.push({ "file":"/data/cgf/huFE71F3-GS01669-DNA_B06.cgf", "name":"huFE71F3-GS01669-DNA_B06", "id":461 });
  cgf_info.cgf.push({ "file":"/data/cgf/huFFAD87-GS01669-DNA_H05.cgf", "name":"huFFAD87-GS01669-DNA_H05", "id":462 });
  cgf_info.cgf.push({ "file":"/data/cgf/huFFB09D-GS01669-DNA_D04.cgf", "name":"huFFB09D-GS01669-DNA_D04", "id":463 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA06984-GS000017231-ASM.cgf", "name":"NA06984-GS000017231-ASM", "id":464 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA06989-GS000017234-ASM.cgf", "name":"NA06989-GS000017234-ASM", "id":465 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA06994-GS000012110-ASM.cgf", "name":"NA06994-GS000012110-ASM", "id":466 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA07000-GS000016078-ASM.cgf", "name":"NA07000-GS000016078-ASM", "id":467 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA07029-GS000013213-ASM.cgf", "name":"NA07029-GS000013213-ASM", "id":468 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA07346-GS000012113-ASM.cgf", "name":"NA07346-GS000012113-ASM", "id":469 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA07347-GS000012580-ASM.cgf", "name":"NA07347-GS000012580-ASM", "id":470 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA07349-GS000012109-ASM.cgf", "name":"NA07349-GS000012109-ASM", "id":471 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA10831-GS000016048-ASM.cgf", "name":"NA10831-GS000016048-ASM", "id":472 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA10837-GS000016047-ASM.cgf", "name":"NA10837-GS000016047-ASM", "id":473 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA10838-GS000016046-ASM.cgf", "name":"NA10838-GS000016046-ASM", "id":474 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA10839-GS000016380-ASM.cgf", "name":"NA10839-GS000016380-ASM", "id":475 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA10840-GS000016542-ASM.cgf", "name":"NA10840-GS000016542-ASM", "id":476 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA10843-GS000016541-ASM.cgf", "name":"NA10843-GS000016541-ASM", "id":477 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA10845-GS000016540-ASM.cgf", "name":"NA10845-GS000016540-ASM", "id":478 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA10852-GS000016045-ASM.cgf", "name":"NA10852-GS000016045-ASM", "id":479 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA10855-GS000016539-ASM.cgf", "name":"NA10855-GS000016539-ASM", "id":480 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA10856-GS000016538-ASM.cgf", "name":"NA10856-GS000016538-ASM", "id":481 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA10861-GS000016044-ASM.cgf", "name":"NA10861-GS000016044-ASM", "id":482 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA10864-GS000016043-ASM.cgf", "name":"NA10864-GS000016043-ASM", "id":483 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA11829-GS000016042-ASM.cgf", "name":"NA11829-GS000016042-ASM", "id":484 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA11830-GS000016041-ASM.cgf", "name":"NA11830-GS000016041-ASM", "id":485 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA11831-GS000016040-ASM.cgf", "name":"NA11831-GS000016040-ASM", "id":486 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA11832-GS000016039-ASM.cgf", "name":"NA11832-GS000016039-ASM", "id":487 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA11893-GS000016537-ASM.cgf", "name":"NA11893-GS000016537-ASM", "id":488 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA11894-GS000016470-ASM.cgf", "name":"NA11894-GS000016470-ASM", "id":489 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA11919-GS000016038-ASM.cgf", "name":"NA11919-GS000016038-ASM", "id":490 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA11920-GS000016037-ASM.cgf", "name":"NA11920-GS000016037-ASM", "id":491 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA11930-GS000016036-ASM.cgf", "name":"NA11930-GS000016036-ASM", "id":492 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA11931-GS000016469-ASM.cgf", "name":"NA11931-GS000016469-ASM", "id":493 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA11994-GS000016468-ASM.cgf", "name":"NA11994-GS000016468-ASM", "id":494 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA11995-GS000016467-ASM.cgf", "name":"NA11995-GS000016467-ASM", "id":495 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12003-GS000016035-ASM.cgf", "name":"NA12003-GS000016035-ASM", "id":496 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12004-GS000016034-ASM.cgf", "name":"NA12004-GS000016034-ASM", "id":497 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12005-GS000016466-ASM.cgf", "name":"NA12005-GS000016466-ASM", "id":498 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12006-GS000016465-ASM.cgf", "name":"NA12006-GS000016465-ASM", "id":499 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12045-GS000016033-ASM.cgf", "name":"NA12045-GS000016033-ASM", "id":500 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12046-GS000016022-ASM.cgf", "name":"NA12046-GS000016022-ASM", "id":501 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12155-GS000016021-ASM.cgf", "name":"NA12155-GS000016021-ASM", "id":502 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12156-GS000016020-ASM.cgf", "name":"NA12156-GS000016020-ASM", "id":503 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12272-GS000016019-ASM.cgf", "name":"NA12272-GS000016019-ASM", "id":504 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12273-GS000016464-ASM.cgf", "name":"NA12273-GS000016464-ASM", "id":505 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12286-GS000016463-ASM.cgf", "name":"NA12286-GS000016463-ASM", "id":506 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12287-GS000016462-ASM.cgf", "name":"NA12287-GS000016462-ASM", "id":507 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12329-GS000017106-ASM.cgf", "name":"NA12329-GS000017106-ASM", "id":508 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12335-GS000016018-ASM.cgf", "name":"NA12335-GS000016018-ASM", "id":509 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12340-GS000016461-ASM.cgf", "name":"NA12340-GS000016461-ASM", "id":510 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12341-GS000016017-ASM.cgf", "name":"NA12341-GS000016017-ASM", "id":511 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12344-GS000016026-ASM.cgf", "name":"NA12344-GS000016026-ASM", "id":512 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12347-GS000016025-ASM.cgf", "name":"NA12347-GS000016025-ASM", "id":513 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12348-GS000017242-ASM.cgf", "name":"NA12348-GS000017242-ASM", "id":514 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12376-GS000016460-ASM.cgf", "name":"NA12376-GS000016460-ASM", "id":515 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12386-GS000016459-ASM.cgf", "name":"NA12386-GS000016459-ASM", "id":516 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12399-GS000016458-ASM.cgf", "name":"NA12399-GS000016458-ASM", "id":517 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12400-GS000016011-ASM.cgf", "name":"NA12400-GS000016011-ASM", "id":518 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12413-GS000016457-ASM.cgf", "name":"NA12413-GS000016457-ASM", "id":519 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12414-GS000016417-ASM.cgf", "name":"NA12414-GS000016417-ASM", "id":520 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12485-GS000016404-ASM.cgf", "name":"NA12485-GS000016404-ASM", "id":521 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12489-GS000016403-ASM.cgf", "name":"NA12489-GS000016403-ASM", "id":522 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12546-GS000016402-ASM.cgf", "name":"NA12546-GS000016402-ASM", "id":523 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12707-GS000016401-ASM.cgf", "name":"NA12707-GS000016401-ASM", "id":524 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12716-GS000017241-ASM.cgf", "name":"NA12716-GS000017241-ASM", "id":525 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12717-GS000016416-ASM.cgf", "name":"NA12717-GS000016416-ASM", "id":526 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12740-GS000016400-ASM.cgf", "name":"NA12740-GS000016400-ASM", "id":527 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12750-GS000016415-ASM.cgf", "name":"NA12750-GS000016415-ASM", "id":528 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12751-GS000016414-ASM.cgf", "name":"NA12751-GS000016414-ASM", "id":529 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12752-GS000016413-ASM.cgf", "name":"NA12752-GS000016413-ASM", "id":530 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12753-GS000016412-ASM.cgf", "name":"NA12753-GS000016412-ASM", "id":531 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12760-GS000016411-ASM.cgf", "name":"NA12760-GS000016411-ASM", "id":532 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12761-GS000016399-ASM.cgf", "name":"NA12761-GS000016399-ASM", "id":533 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12762-GS000016410-ASM.cgf", "name":"NA12762-GS000016410-ASM", "id":534 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12763-GS000016398-ASM.cgf", "name":"NA12763-GS000016398-ASM", "id":535 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12766-GS000016409-ASM.cgf", "name":"NA12766-GS000016409-ASM", "id":536 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12767-GS000016408-ASM.cgf", "name":"NA12767-GS000016408-ASM", "id":537 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12775-GS000016397-ASM.cgf", "name":"NA12775-GS000016397-ASM", "id":538 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12776-GS000016396-ASM.cgf", "name":"NA12776-GS000016396-ASM", "id":539 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12777-GS000016395-ASM.cgf", "name":"NA12777-GS000016395-ASM", "id":540 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12778-GS000016456-ASM.cgf", "name":"NA12778-GS000016456-ASM", "id":541 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12801-GS000016407-ASM.cgf", "name":"NA12801-GS000016407-ASM", "id":542 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12802-GS000016406-ASM.cgf", "name":"NA12802-GS000016406-ASM", "id":543 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12812-GS000016405-ASM.cgf", "name":"NA12812-GS000016405-ASM", "id":544 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12813-GS000016394-ASM.cgf", "name":"NA12813-GS000016394-ASM", "id":545 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12814-GS000016393-ASM.cgf", "name":"NA12814-GS000016393-ASM", "id":546 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12815-GS000016383-ASM.cgf", "name":"NA12815-GS000016383-ASM", "id":547 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12817-GS000016418-ASM.cgf", "name":"NA12817-GS000016418-ASM", "id":548 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12818-GS000016392-ASM.cgf", "name":"NA12818-GS000016392-ASM", "id":549 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12827-GS000017240-ASM.cgf", "name":"NA12827-GS000017240-ASM", "id":550 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12828-GS000017899-ASM.cgf", "name":"NA12828-GS000017899-ASM", "id":551 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12829-GS000016704-ASM.cgf", "name":"NA12829-GS000016704-ASM", "id":552 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12830-GS000017900-ASM.cgf", "name":"NA12830-GS000017900-ASM", "id":553 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12832-GS000016702-ASM.cgf", "name":"NA12832-GS000016702-ASM", "id":554 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12842-GS000016701-ASM.cgf", "name":"NA12842-GS000016701-ASM", "id":555 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12843-GS000016700-ASM.cgf", "name":"NA12843-GS000016700-ASM", "id":556 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12864-GS000016382-ASM.cgf", "name":"NA12864-GS000016382-ASM", "id":557 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12872-GS000016381-ASM.cgf", "name":"NA12872-GS000016381-ASM", "id":558 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA12873-GS000016699-ASM.cgf", "name":"NA12873-GS000016699-ASM", "id":559 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18497-GS000017238-ASM.cgf", "name":"NA18497-GS000017238-ASM", "id":560 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18498-GS000017237-ASM.cgf", "name":"NA18498-GS000017237-ASM", "id":561 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18499-GS000017224-ASM.cgf", "name":"NA18499-GS000017224-ASM", "id":562 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18500-GS000017223-ASM.cgf", "name":"NA18500-GS000017223-ASM", "id":563 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18501-GS000017371-ASM.cgf", "name":"NA18501-GS000017371-ASM", "id":564 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18502-GS000017172-ASM.cgf", "name":"NA18502-GS000017172-ASM", "id":565 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18503-GS000017173-ASM.cgf", "name":"NA18503-GS000017173-ASM", "id":566 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18504-GS000017182-ASM.cgf", "name":"NA18504-GS000017182-ASM", "id":567 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18505-GS000017185-ASM.cgf", "name":"NA18505-GS000017185-ASM", "id":568 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18506-GS000017030-ASM.cgf", "name":"NA18506-GS000017030-ASM", "id":569 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18507-GS000017029-ASM.cgf", "name":"NA18507-GS000017029-ASM", "id":570 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18508-GS000017028-ASM.cgf", "name":"NA18508-GS000017028-ASM", "id":571 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18521-GS000017023-ASM.cgf", "name":"NA18521-GS000017023-ASM", "id":572 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18522-GS000017026-ASM.cgf", "name":"NA18522-GS000017026-ASM", "id":573 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18870-GS000017227-ASM.cgf", "name":"NA18870-GS000017227-ASM", "id":574 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18871-GS000017226-ASM.cgf", "name":"NA18871-GS000017226-ASM", "id":575 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18872-GS000017119-ASM.cgf", "name":"NA18872-GS000017119-ASM", "id":576 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18909-GS000017025-ASM.cgf", "name":"NA18909-GS000017025-ASM", "id":577 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18911-GS000017024-ASM.cgf", "name":"NA18911-GS000017024-ASM", "id":578 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18916-GS000017022-ASM.cgf", "name":"NA18916-GS000017022-ASM", "id":579 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18917-GS000017027-ASM.cgf", "name":"NA18917-GS000017027-ASM", "id":580 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18923-GS000017262-ASM.cgf", "name":"NA18923-GS000017262-ASM", "id":581 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18924-GS000017261-ASM.cgf", "name":"NA18924-GS000017261-ASM", "id":582 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18930-GS000017044-ASM.cgf", "name":"NA18930-GS000017044-ASM", "id":583 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18933-GS000017045-ASM.cgf", "name":"NA18933-GS000017045-ASM", "id":584 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18934-GS000017046-ASM.cgf", "name":"NA18934-GS000017046-ASM", "id":585 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA18935-GS000017047-ASM.cgf", "name":"NA18935-GS000017047-ASM", "id":586 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19093-GS000017050-ASM.cgf", "name":"NA19093-GS000017050-ASM", "id":587 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19097-GS000017053-ASM.cgf", "name":"NA19097-GS000017053-ASM", "id":588 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19098-GS000017043-ASM.cgf", "name":"NA19098-GS000017043-ASM", "id":589 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19100-GS000017041-ASM.cgf", "name":"NA19100-GS000017041-ASM", "id":590 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19107-GS000017040-ASM.cgf", "name":"NA19107-GS000017040-ASM", "id":591 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19109-GS000017039-ASM.cgf", "name":"NA19109-GS000017039-ASM", "id":592 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19113-GS000017038-ASM.cgf", "name":"NA19113-GS000017038-ASM", "id":593 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19114-GS000017037-ASM.cgf", "name":"NA19114-GS000017037-ASM", "id":594 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19115-GS000017036-ASM.cgf", "name":"NA19115-GS000017036-ASM", "id":595 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19116-GS000017035-ASM.cgf", "name":"NA19116-GS000017035-ASM", "id":596 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19117-GS000017051-ASM.cgf", "name":"NA19117-GS000017051-ASM", "id":597 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19118-GS000017034-ASM.cgf", "name":"NA19118-GS000017034-ASM", "id":598 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19120-GS000017052-ASM.cgf", "name":"NA19120-GS000017052-ASM", "id":599 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19130-GS000017055-ASM.cgf", "name":"NA19130-GS000017055-ASM", "id":600 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19137-GS000017137-ASM.cgf", "name":"NA19137-GS000017137-ASM", "id":601 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19138-GS000017189-ASM.cgf", "name":"NA19138-GS000017189-ASM", "id":602 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19139-GS000017188-ASM.cgf", "name":"NA19139-GS000017188-ASM", "id":603 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19143-GS000017054-ASM.cgf", "name":"NA19143-GS000017054-ASM", "id":604 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19144-GS000017136-ASM.cgf", "name":"NA19144-GS000017136-ASM", "id":605 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19145-GS000017133-ASM.cgf", "name":"NA19145-GS000017133-ASM", "id":606 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19146-GS000017131-ASM.cgf", "name":"NA19146-GS000017131-ASM", "id":607 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19147-GS000018623-ASM.cgf", "name":"NA19147-GS000018623-ASM", "id":608 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19148-GS000018624-ASM.cgf", "name":"NA19148-GS000018624-ASM", "id":609 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19152-GS000017413-ASM.cgf", "name":"NA19152-GS000017413-ASM", "id":610 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19153-GS000017412-ASM.cgf", "name":"NA19153-GS000017412-ASM", "id":611 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19154-GS000017411-ASM.cgf", "name":"NA19154-GS000017411-ASM", "id":612 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19159-GS000017410-ASM.cgf", "name":"NA19159-GS000017410-ASM", "id":613 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19160-GS000017259-ASM.cgf", "name":"NA19160-GS000017259-ASM", "id":614 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19161-GS000017409-ASM.cgf", "name":"NA19161-GS000017409-ASM", "id":615 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19171-GS000017280-ASM.cgf", "name":"NA19171-GS000017280-ASM", "id":616 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19172-GS000017279-ASM.cgf", "name":"NA19172-GS000017279-ASM", "id":617 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19173-GS000017257-ASM.cgf", "name":"NA19173-GS000017257-ASM", "id":618 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19174-GS000017278-ASM.cgf", "name":"NA19174-GS000017278-ASM", "id":619 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19186-GS000017260-ASM.cgf", "name":"NA19186-GS000017260-ASM", "id":620 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19189-GS000017277-ASM.cgf", "name":"NA19189-GS000017277-ASM", "id":621 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19190-GS000017276-ASM.cgf", "name":"NA19190-GS000017276-ASM", "id":622 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19191-GS000017275-ASM.cgf", "name":"NA19191-GS000017275-ASM", "id":623 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19200-GS000017898-ASM.cgf", "name":"NA19200-GS000017898-ASM", "id":624 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19201-GS000017244-ASM.cgf", "name":"NA19201-GS000017244-ASM", "id":625 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19202-GS000017174-ASM.cgf", "name":"NA19202-GS000017174-ASM", "id":626 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19210-GS000017274-ASM.cgf", "name":"NA19210-GS000017274-ASM", "id":627 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19211-GS000017273-ASM.cgf", "name":"NA19211-GS000017273-ASM", "id":628 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19221-GS000017272-ASM.cgf", "name":"NA19221-GS000017272-ASM", "id":629 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19222-GS000017270-ASM.cgf", "name":"NA19222-GS000017270-ASM", "id":630 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19236-GS000017269-ASM.cgf", "name":"NA19236-GS000017269-ASM", "id":631 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19238-GS000017268-ASM.cgf", "name":"NA19238-GS000017268-ASM", "id":632 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19239-GS000017267-ASM.cgf", "name":"NA19239-GS000017267-ASM", "id":633 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19240-GS000018625-ASM.cgf", "name":"NA19240-GS000018625-ASM", "id":634 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19247-GS000017266-ASM.cgf", "name":"NA19247-GS000017266-ASM", "id":635 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19249-GS000017265-ASM.cgf", "name":"NA19249-GS000017265-ASM", "id":636 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19256-GS000017258-ASM.cgf", "name":"NA19256-GS000017258-ASM", "id":637 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19257-GS000017264-ASM.cgf", "name":"NA19257-GS000017264-ASM", "id":638 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19258-GS000017263-ASM.cgf", "name":"NA19258-GS000017263-ASM", "id":639 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19314-GS000017175-ASM.cgf", "name":"NA19314-GS000017175-ASM", "id":640 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19397-GS000017176-ASM.cgf", "name":"NA19397-GS000017176-ASM", "id":641 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19398-GS000017243-ASM.cgf", "name":"NA19398-GS000017243-ASM", "id":642 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19399-GS000017177-ASM.cgf", "name":"NA19399-GS000017177-ASM", "id":643 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19404-GS000017239-ASM.cgf", "name":"NA19404-GS000017239-ASM", "id":644 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19428-GS000017187-ASM.cgf", "name":"NA19428-GS000017187-ASM", "id":645 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19429-GS000017186-ASM.cgf", "name":"NA19429-GS000017186-ASM", "id":646 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19434-GS000016668-ASM.cgf", "name":"NA19434-GS000016668-ASM", "id":647 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19435-GS000016558-ASM.cgf", "name":"NA19435-GS000016558-ASM", "id":648 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19440-GS000016557-ASM.cgf", "name":"NA19440-GS000016557-ASM", "id":649 });
  cgf_info.cgf.push({ "file":"/data/cgf/NA19443-GS000016556-ASM.cgf", "name":"NA19443-GS000016556-ASM", "id":650 });


  cgf_info.id = {};
  for (var idx=0; idx<cgf_info.cgf.length; idx++) {
    cgf_info.id[ cgf_info.cgf[idx].name ] = idx;
  }

  cgf_info["CGFVersion"] = "0.1.0";
  cgf_info["CGFLibVersion"] = "0.1.0";
  cgf_info["PathCount"] = 863;
  cgf_info["StepPerPath"] = [
     5433, 11585, 7112, 7550, 13094, 10061, 15111, 13212, 14838, 7361, 8565, 8238, 21058, 15318, 9982, 14543,
    20484, 11704, 9056, 29572, 3032, 58941, 13626, 13753, 10082, 19756, 9669, 18011, 17221, 16418, 6572, 10450,
    653, 1, 1, 43, 4603, 4524, 17225, 5245, 9951, 5416, 18877, 6467, 14301, 7627, 11539, 16593,
    21475, 19845, 11886, 19126, 30932, 16774, 11607, 37511, 1368, 9016, 14132, 15803, 6847, 26570, 19594, 17082,
    10529, 20354, 17716, 9931, 19189, 14703, 8418, 8231, 17045, 7804, 12459, 23570, 20025, 8246, 24611, 10263,
    17693, 11001, 7904, 5629, 32719, 19083, 565, 3431, 20757, 13319, 5383, 9608, 10026, 16921, 14381, 29377,
    6845, 8754, 6367, 21554, 7707, 18707, 4227, 2345, 16932, 19091, 15332, 23909, 32173, 10128, 9612, 24819,
    9782, 21619, 22599, 5851, 16177, 24645, 24453, 14657, 3551, 19209, 17178, 6784, 22677, 10729, 4764, 18388,
    11981, 5804, 12040, 29022, 9918, 17574, 4842, 16740, 11327, 16335, 1542, 416, 23880, 6126, 8255, 16187,
    20267, 23705, 17658, 21050, 14728, 14705, 2708, 9599, 1327, 17097, 6536, 3446, 7194, 13517, 6740, 12960,
    8454, 15276, 6666, 10736, 7497, 7113, 13394, 16658, 7897, 10893, 15843, 24193, 12589, 10989, 7735, 7704,
    6591, 26835, 12945, 19129, 12707, 14282, 6739, 5660, 7363, 17599, 20166, 15899, 5832, 18674, 15349, 10225,
    13863, 25249, 32580, 20511, 13259, 14135, 3468, 71, 25343, 27513, 14097, 21456, 9860, 13680, 6387, 10838,
    4120, 21815, 5451, 14460, 8533, 24975, 24610, 25300, 11590, 19404, 8688, 32414, 7729, 19437, 6621, 10118,
    17649, 24182, 10736, 21411, 6710, 17505, 4790, 22874, 15243, 14561, 17381, 7292, 13961, 20750, 12771, 17639,
    5133, 16978, 18906, 16519, 15821, 13209, 882, 4225, 31741, 15233, 1182, 13597, 6528, 11710, 13632, 16991,
    5455, 37078, 22890, 16898, 6764, 20266, 7277, 6180, 8009, 24144, 22877, 12483, 21662, 12287, 19473, 20872,
    11085, 11566, 16415, 34070, 16922, 13794, 14120, 8663, 7451, 11295, 13748, 3815, 7213, 7030, 38651, 6143,
    12781, 5883, 5178, 11753, 15562, 22214, 22047, 4132, 16117, 3941, 144, 4865, 400, 25489, 22288, 30139,
    3706, 11083, 19909, 24752, 4171, 19061, 35002, 14079, 794, 29730, 3892, 12776, 3515, 15587, 14919, 14827,
    11010, 13427, 13368, 11662, 21111, 13834, 24662, 10333, 6684, 8376, 25611, 10830, 17440, 17699, 9856, 3300,
    23551, 7908, 24000, 7739, 13746, 5876, 13653, 11619, 105, 227, 12689, 19087, 11490, 35461, 6928, 11137,
    6317, 19717, 18677, 2636, 10982, 28108, 11243, 14787, 10618, 12904, 7678, 4053, 8783, 21899, 18003, 16798,
    16058, 8543, 15728, 7511, 16071, 18591, 25102, 17085, 16227, 5457, 29901, 6958, 5306, 12761, 2290, 4222,
    15593, 1523, 10990, 23625, 2365, 14954, 7597, 9733, 12983, 17099, 7155, 17446, 7771, 24670, 22012, 9790,
    17944, 16958, 6352, 22341, 6025, 12803, 18803, 16509, 19724, 13970, 23963, 7842, 9501, 16725, 20807, 9222,
    7462, 5182, 22155, 9365, 20144, 11012, 8142, 1490, 180, 546, 1, 1, 15, 550, 4865, 7015,
    20266, 7250, 11850, 10403, 13346, 5036, 7311, 10212, 9994, 12206, 21611, 12006, 13925, 10860, 19459, 12846,
    17584, 11203, 1904, 7356, 5714, 14022, 11522, 3238, 10867, 22206, 19356, 3286, 381, 14758, 7681, 18901,
    6319, 11569, 13319, 2602, 1, 12601, 5388, 8544, 32551, 13246, 23124, 16676, 10420, 16083, 23002, 4756,
    13393, 4473, 10500, 8904, 9750, 4253, 7078, 3459, 24069, 12012, 16737, 10252, 5577, 17329, 11901, 19092,
    9991, 28650, 8063, 13688, 21339, 17049, 4291, 15046, 21055, 27571, 19581, 5339, 1, 2796, 15653, 6733,
    5702, 9463, 8431, 7485, 17429, 7445, 33236, 10017, 15088, 16390, 18985, 3047, 29163, 8290, 8000, 26700,
    10459, 15540, 11802, 16858, 12184, 8407, 15777, 9945, 7774, 20407, 5030, 20355, 4994, 11256, 9088, 5210,
    703, 31263, 9981, 8655, 12869, 6059, 5323, 19308, 6962, 10252, 14659, 16466, 18159, 25083, 8822, 14458,
    13654, 20804, 8472, 20356, 9936, 2048, 7595, 10099, 4973, 9834, 18782, 13534, 16861, 1, 1, 1,
    1, 449, 13648, 8140, 8894, 4307, 12796, 7164, 5979, 18211, 19843, 2279, 5677, 13654, 16553, 17021,
    10676, 13343, 11629, 19081, 8331, 7079, 7216, 33870, 9290, 20014, 12554, 4179, 9303, 12659, 8980, 13317,
    17551, 1, 1, 1, 1, 91, 16293, 33478, 7694, 4755, 4736, 21768, 13932, 14148, 12245, 5458,
    10017, 15321, 10317, 11761, 9101, 13816, 21162, 17182, 5312, 19338, 8096, 10791, 6468, 20877, 6861, 3000,
    11596, 1, 1, 1, 1, 650, 9508, 9670, 6240, 919, 7453, 25276, 10122, 2914, 3833, 18009,
    12803, 23978, 730, 17531, 13228, 383, 818, 20600, 9375, 4772, 6376, 13251, 9675, 14940, 19964, 17117,
    15125, 29145, 9758, 7794, 8325, 3452, 13738, 9153, 15025, 13310, 2344, 1, 1932, 22098, 16158, 2693,
    36810, 14074, 7919, 4845, 19451, 10051, 10058, 11572, 5454, 5493, 11041, 11843, 15854, 19846, 17827, 125,
    1653, 21451, 21850, 1084, 9274, 12463, 8800, 10895, 28728, 2071, 9705, 5530, 5548, 10683, 15160, 14696,
    1860, 22145, 10747, 16523, 5517, 9195, 13344, 12, 1771, 23326, 30739, 18023, 25450, 18584, 21768, 9509,
    10948, 10287, 21091, 7440, 17747, 19563, 23601, 23077, 347, 7918, 12998, 13442, 559, 2780, 15135, 11458,
    9677, 1431, 16004, 5610, 9780, 11468, 7764, 8969, 10185, 19284, 16238, 11893, 23036, 13336, 3819, 12729,
    1268, 1, 8908, 8382, 11966, 16626, 1577, 16554, 12901, 20235, 6003, 7836, 17926, 1, 1, 1214,
    534, 1, 4755, 30050, 11265, 18230, 16716, 7896, 7554, 11599, 21326, 1, 1, 1, 1, 2745,
    11905, 4602, 8007, 14401, 9459, 20828, 12737, 11643, 16587, 4104, 6858, 5235, 6576, 13137, 29283, 8472,
    9959, 11291, 16995, 8588, 23499, 18569, 14851, 10837, 14462, 10224, 1492, 3714, 5149, 10944, 13980, 6118,
    6368, 29977, 5799, 12454, 4748, 18033, 14477, 3916, 18518, 28427, 15228, 29028, 6516, 11944, 15846, 8098,
    6040, 18525, 26363, 1, 1045, 12490, 1, 361, 4758, 14711, 4019, 5647, 721, 181, 35
  ];

  return cgf_info;
}
setup_cgf_info();

function tile_concordance_slice(set_a, set_b, lvl) {
}

function tile_concordance(set_a, set_b, lvl) {
  var A = [], B = [];

  lvl=((typeof(lvl)==="undefined")?2:lvl);
  if (lvl<0) { lvl=0; }
  if (lvl>2) { lvl=2; }

  for (var i=0; i<set_a.length; i++) {
    if (typeof(set_a[i])==="string") {
      if (set_a[i] in cgf_info.id) {
        A.push(cgf_info.id[set_a[i]]);
      } else if ((set_a[i]>=0) && (set_a[i]<cgf_info.id.length)) {
        A.push(set_a[i]);
      }
    }
  }

  for (var i=0; i<set_b.length; i++) {
    if (typeof(set_b[i])==="string") {
      if (set_b[i] in cgf_info.id) {
        B.push(cgf_info.id[set_b[i]]);
      } else if ((set_b[i]>=0) && (set_b[i]<cgf_info.id.length)) {
        B.push(set_b[i]);
      }
    }
  }

  var rstr = "";
  //var rstr = muduk_cgf_tile_concordance(A,B,lvl);

  var r = {};
  try {
    r = JSON.parse(rstr);
  } catch(err) {
    r["result"] = "error: parse error" + String(err);
  }

  return r;
}

function help() {
  print("muduk server");
}

function query(q) {
  var qobj = {};
  var robj = {};
  try {
    qobj = JSON.parse(q);
  } catch(err) {
    return err;
  }

  if ("request" in qobj) {
    print("request: " + String(qobj["request"]));
  }

  return JSON.stringify(robj);
}

function muduk_return(q, indent) {
  indent = ((typeof(indent)==="undefined") ? '' : indent);
  if (typeof(q)==="undefined") { return ""; }
  if (typeof(q)==="object") {
    var s = "";
    try {
      s = JSON.stringify(q, null, indent);
    } catch(err) {
    }
    return s;
  }
  if (typeof(q)==="string") { return q; }
  if (typeof(q)==="number") { return q; }
  return "";
}

