#include "utils.h"

int func_win_perm(LPCSTR filepath,DWORD perms)
{
    PACL pOldDACL = NULL;
    PACL pNewDACL = NULL;
    PSECURITY_DESCRIPTOR pSD = NULL;

    EXPLICIT_ACCESS ea;
    DWORD res;

    res = GetNamedSecurityInfoA(
              filepath,
              SE_FILE_OBJECT,
              DACL_SECURITY_INFORMATION,
              NULL,
              NULL,
              &pOldDACL,
              NULL,
              &pSD
          );
    if(res != ERROR_SUCCESS) {
        printf("GetNamedSecurityInfoA\n");
        return 1;
    }
    TOKEN_USER *tu = NULL;
    DWORD tuLen = 0;
    HANDLE hTok;

    OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hTok);
    GetTokenInformation(hTok,TokenUser,NULL,0,&tuLen);

    tu = (TOKEN_USER*)malloc(tuLen);
    GetTokenInformation(hTok,TokenUser,tu,tuLen,&tuLen);
    CloseHandle(hTok);

    ZeroMemory(&ea, sizeof(ea));
    ZeroMemory(&ea.Trustee, sizeof(ea.Trustee));

    ea.grfAccessPermissions = perms;
    ea.grfAccessMode = SET_ACCESS;
    ea.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
    ea.Trustee.pMultipleTrustee = NULL;
    ea.Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
    ea.Trustee.ptstrName = (LPSTR)tu->User.Sid;

    res = SetEntriesInAcl(1, &ea, pOldDACL, &pNewDACL);
    if(res != ERROR_SUCCESS) {
        printf("SetEntriesInAcl\n");
        return 1;
    }


    res = SetNamedSecurityInfoA(
              (LPSTR)filepath,
              SE_FILE_OBJECT,
              DACL_SECURITY_INFORMATION,
              NULL,
              NULL,
              pNewDACL,
              NULL

          );

    LocalFree(pSD);
    LocalFree(pNewDACL);
    LocalFree(pOldDACL);
    free(tu);

    if(res != ERROR_SUCCESS) {
        printf("SetNamedSecurityInfoA\n");
        return 1;
    }

    return 1;
}

int fdown(char **args)
{
#ifdef _WIN32
    PSID pNewOwnerSid = NULL;
    PSID pNewGroupSid = NULL;

    DWORD sidSize = 0, domainSize = 0;
    SID_NAME_USE sidType;
    // ----- OWNER -----

    LookupAccountNameA(NULL,args[2],NULL,&sidSize,NULL,&domainSize,&sidType);
    if (sidSize == 0 || domainSize == 0) {
        printf("Invalid user: %s\n", args[2]);
        return 1;
    }
    pNewOwnerSid = malloc(sidSize);
    char *domain = malloc(domainSize);
    if (domain == NULL || pNewOwnerSid == NULL) {
        printf("Cannot Malloc!\n");
        return 1;
    }
    // ----- GROUP -----

    DWORD groupSidSize = 0, groupDomainSize = 0;
    SID_NAME_USE groupSidUse;

    LookupAccountNameA(NULL, args[3],NULL,&groupSidSize,NULL,&groupDomainSize,&groupSidUse);
    if (groupSidSize == 0 || groupDomainSize == 0) {
        printf("Invalid group: %s\n", args[3]);
        free(domain);
        free(pNewOwnerSid);
        return 1;
    }
    pNewGroupSid = malloc(groupSidSize);
    char *groupDomain = malloc(groupDomainSize);
    if (groupDomain == NULL || pNewGroupSid  == NULL) {
        printf("Cannot Malloc!\n");
        free(domain);
        free(pNewOwnerSid);
        return 1;
    }



    if (!LookupAccountNameA(NULL, args[2], pNewOwnerSid, &sidSize, domain, &domainSize, &sidType)) {
        printf("Error: Cannot find user or group - '%s' -\n",args[2]);
        free(domain);
        free(pNewOwnerSid);
        return 1;
    }

    if (!LookupAccountNameA(NULL, args[3], pNewGroupSid, &groupSidSize, groupDomain, &groupDomainSize, &groupSidUse)) {
        printf("Group '%s' not found!\n",args[3]);
        free(domain);
        free(pNewOwnerSid);
        free(groupDomain);
        free(pNewGroupSid);
        return 1;
    }



    if(pNewOwnerSid != NULL) {
        DWORD dwResult = SetNamedSecurityInfoA(
                             args[1],
                             SE_FILE_OBJECT,
                             OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION,
                             pNewOwnerSid,
                             pNewGroupSid, // No primary group change
                             NULL, // No DACL change
                             NULL  // No SACL change
                         );
        if(dwResult == ERROR_SUCCESS) {
            printf("Ownership Changed Successfully!\n");
        } else {
            printf("Cannot Change Owner ship\n");
            return 1;
        }

    }
    free(domain);
    free(pNewOwnerSid);
    free(groupDomain);
    free(pNewGroupSid);
    return 1;
#else
    if (n < 3) {
        printf("%sUsage: chown <subject> <owner> <group>%s\n", BOLDRED, RESET);
        return 1;
    }
    if (chown(args[1], args[2], args[3]) == -1) {
        printf("%sCannot Change Permission!%s\n", BOLDRED, RESET);
        return 1;
    }
    return 1;
#endif
}

int unix_ls(void)
{
    char filename[1024];
    DIR *dir;
    struct dirent *dirp;
    dir = opendir(".");

    if (dir == NULL) {
        printf("opendir\n");
        return 1;
    }

    while ((dirp = readdir(dir)) != NULL) {
        snprintf(filename, sizeof(filename), "%s", dirp->d_name);
        struct stat st;
        if (stat(filename, &st) != 0) {
            continue; // Cannot read stat, continue loop
        }
        struct tm *tm;
        tm = localtime(&st.st_mtime);

        char type = S_ISDIR(st.st_mode) ? 'd' : '-';

        printf(
            "%c %10ld %02d/%02d/%04d %s\n",
            type,
            st.st_size,
            tm->tm_mon + 1,
            tm->tm_mday,
            tm->tm_year + 1900,
            dirp->d_name);
    }
    closedir(dir);

    return 1;
}

int winls(void)
{
    WIN32_FIND_DATA findfiledata;
    HANDLE hfind;

    hfind = FindFirstFile("*", &findfiledata);
    if (hfind == INVALID_HANDLE_VALUE) {
        printf("Cannot List.\n");
        return 1;
    }

    do {
        // type file or dir
        DWORD attr = findfiledata.dwFileAttributes;
        const char *type = (attr & FILE_ATTRIBUTE_DIRECTORY) ? "d" : "-";

        // size of file, (dir is 0)
        DWORD size = findfiledata.nFileSizeLow;

        // Convert Modification data
        FILETIME local;
        SYSTEMTIME st;
        FileTimeToLocalFileTime(&findfiledata.ftLastWriteTime, &local);
        FileTimeToSystemTime(&local, &st);

        // Final Print
        printf("%s %10lu  %02d/%02d/%04d  %s\n",
               type,
               size,
               st.wDay, st.wMonth, st.wYear,
               findfiledata.cFileName);

    } while (FindNextFile(hfind, &findfiledata));

    FindClose(hfind);
    return 1;
}

int uget_uname(
    int buffsize,
    const char *path,
    const char *ErrorMessage,
    const char *Message,
    bool PFcpu_t
)
{
    char buffer[buffsize];
    FILE *file = fopen(path,"r");

    if(file == NULL) {
        printf("%s: %s\n",ErrorMessage,path);
        return 1;
    }
    if(!PFcpu_t) {
        if (fgets(buffer,buffsize,file)) {
            buffer[strcspn(buffer, "\n")] = 0;
            printf("%s: %s\n",Message,buffer);
        }
        fclose(file);
        return 1;
    } else {
        while(fgets(buffer,buffsize,file)) {
            if(strstr(buffer,"model name") != NULL) {
                char* color_pos = strchr(buffer,':');
                if(color_pos != NULL) {
                    char * model_name = color_pos + 2;
                    model_name[strcspn(model_name,"\n")] = 0;
                    printf("%s%s\n",Message,model_name);
                    fclose(file);
                    return 1;
                }
            }
        }
    }
    return 1;
}

int error_set(char *msg)
{
    fprintf(stderr,"shell: %s",msg);
    return -1;
}