#ifndef FILEPATHS_H
#define FILEPATHS_H

#include <vector>
#include <QString>
#include <QList>

const std::vector<QString> paths {"^.*oam-config.xml$",
                                  "^.*/config/fmwconfig/oam-config.xml$",
                                  "^.*adf-config.xml$",
                                  "^.*/coherence./lib/coherence.jar$",
                                  "^.*/coherence./lib/security/coherence-login.jar$",
                                  "^.*tangosol-coherence.xml$",
                                  "^.*tangosol-coherence-override.xml$",
                                  "^.*/user_projects/domains/./config/config.xml$",
                                  "^.*/server/lib/weblogic.jar$",
                                  "^.*user_projects/domains/wl_server/startWebLogic.*$",
                                  "^.*user_projects/domains/wl_server/bin/startWebLogic.*$",
                                  "^.*/startWebLogic.*$",
                                  "^.*startPIA.cmd$",
                                  "^.*stopPIA.cmd$",
                                  "^.*startPIA.sh$",
                                  "^.*stopPIA.sh$",
                                  "^.*/user_projects/domains/EPMSystem.*$",
                                  "^.*/user_projects/domains/epmsystem.*$",
                                  "^.*/servers/wls_forms/security/boot.properties$",
                                  "^.*/servers/wls_reports/security/boot.properties$",
                                  "^.*/servers/wls_forms/applications$",
                                  "^.*/servers/wls_reports/applications$",
                                  "^.*/formsweb.cfg$",
                                  "^.*biee-domain.xml.*$",
                                  "^.*/user_projects/domains/bifoundation_domain/bin/startWebLogic.*$",
                                  "^.*nqsconfig.ini.*$",
                                  "^.*oracle-bi-shared.jar.*$",
                                  "^.*/bin/startessbase.sh$",
                                  "^.*/bin/stopessbase.sh$",
                                  "^.*/bin/startessbase.bat$",
                                  "^.*/bin/stopessbase.bat$",
                                  "^.*/essbaseserver/essbaseserver.setessbaseenv$",
                                  "^.*/bin/oracle.exe$",
                                  "^.*/bin/tnslsnr.exe$",
                                  "^.*/bin/lsnrctl.exe$",
                                  "^.*/bin/oracle$",
                                  "^.*/bin/tnslsnr$",
                                  "^.*/bin/lsnrctl$",
                                  "^.*oes-common.jar.*$",
                                  "^.*/oessm/oescommon/oes-common.jar$",
                                  "^.*/oessm/./oes-common.jar$",
                                  "^.*oes_client_home$",
                                  "^.*ggsci.*$"};

void getPatterns(const QString& username, const QString& basic_auth_hash, QList<QString>& patterns);

#endif // FILEPATHS_H
