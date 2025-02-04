export interface Profile {
    login: string;
    email: string;
    phone: string;
    nickname: string;
    admin: boolean;
    visitorActions: number[];
    workerActions: number[];
}
