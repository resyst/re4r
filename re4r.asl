// re4 autosplitter

state ("re4")
{
    long GameElapsedTime : 0x0D234048, 0x20, 0x18;
    long DemoSpendingTime : 0x0D234048, 0x20, 0x20;
    long PauseSpendingTime : 0x0D234048, 0x20, 0x30;
    long ChapterStartTime : 0x0D20FF80, 0x20, 0x10, 0x18;
    int PrevChapterStatsCount : 0x0D20FF80, 0x38, 0x18;
    int ChapterID : 0x0D21B2C8, 0x154;
    int CutSceneId : 0x0D21B2C8, 0x17C;
    int KeyItemsCount : 0x0D2320C8, 0x40, 0x30, 0x318, 0x98, 0x68;
    int InteractionId : 0x0D23E618, 0x90, 0x10, 0x20, 0x10, 0x10, 0x20;
}

startup
{
    vars.Done = new HashSet<string>();
    vars.ChapterEnd = false;

    settings.Add("_1", true, "Chapter 1");
    settings.Add("s10155", true, "Lodge Defenestration", "_1");
    settings.Add("g537658525", true, "Village", "_1");
    settings.Add("s10011", true, "Bingo", "_1");
    settings.Add("k119235200", true, "Wooden Cog", "_1");
    settings.Add("c21200", true, "End", "_1");

    settings.Add("_2", true, "Chapter 2");
    settings.Add("s10128", true, "Merchant", "_2");
    settings.Add("k119203200", true, "Hexagonal Emblem", "_2");
    settings.Add("g537658882", true, "Eagle Gate", "_2");
    settings.Add("c21300", true, "End", "_2");

    settings.Add("_3", true, "Chapter 3");
    settings.Add("s10137", true, "Church", "_3");
    settings.Add("g537659582", true, "Merchant Door", "_3");
    settings.Add("k119273600", true, "Boat Fuel", "_3");
    settings.Add("s10018", true, "Del Lago", "_3");
    settings.Add("c22100", true, "End", "_3");

    settings.Add("_4", true, "Chapter 4");
    settings.Add("k119230400", true, "Blasphemer's Head", "_4");
    settings.Add("k119232000", true, "Apostate's Head", "_4");
    settings.Add("s10022", true, "El Gigante", "_4");
    settings.Add("s20123", true, "El Gigante (Dead)", "_4");
    settings.Add("c22200", true, "End", "_4");

    settings.Add("_5", true, "Chapter 5");
    settings.Add("s10026", true, "Luis' Cabin", "_5");
    settings.Add("c22300", true, "End", "_5");

    settings.Add("_6", true, "Chapter 6");
    settings.Add("s10125", true, "Chainsaw Sisters", "_6");
    settings.Add("g539099813", true, "Crank Door", "_6");
    settings.Add("s10028", true, "Mendez", "_6");
    settings.Add("s20111", true, "Mendez 2", "_6");
    settings.Add("s20112", true, "Mendez (Dead)", "_6");
    settings.Add("c23100", true, "End", "_6");

    settings.Add("_7", true, "Chapter 7");
    settings.Add("s10104", true, "Catapults", "_7");
    settings.Add("s10032", true, "Salazar Greeting", "_7");
    settings.Add("k119206400", true, "Halo Wheel", "_7");
    settings.Add("c23200", true, "End", "_7");

    settings.Add("_8", true, "Chapter 8");
    settings.Add("s10036", true, "Ada vs Leon", "_8");
    settings.Add("s20113", true, "Gigante Ramparts", "_8");
    settings.Add("c23300", true, "End", "_8");

    settings.Add("_9", true, "Chapter 9");
    settings.Add("g537594702", true, "Flag 1", "_9");
    settings.Add("g537594704", true, "Flag 2", "_9");
    settings.Add("g537594703", true, "Flag 3", "_9");
    settings.Add("g537593074", true, "Grand Hall", "_9");
    settings.Add("k119225600", true, "Serpent Head", "_9");
    settings.Add("k119222400", true, "Lion Head", "_9");
    settings.Add("k119224000", true, "Goat Head", "_9");
    settings.Add("c24100", true, "End", "_9");

    settings.Add("_10", true, "Chapter 10");
    settings.Add("g537595538", true, "Twin Garradors", "_10");
    settings.Add("s10041", true, "Throne Room", "_10");
    settings.Add("s10042", true, "Verdugo", "_10");
    settings.Add("c24200", true, "End", "_10");

    settings.Add("_11", true, "Chapter 11");
    settings.Add("k119236800", true, "Dynamite", "_11");
    settings.Add("g539099774", true, "Furnace Exit", "_11");
    settings.Add("s10047", true, "Jump", "_11");
    settings.Add("s10048", true, "Goodbye Luis", "_11");
    settings.Add("c24300", true, "End", "_11");

    settings.Add("_12", true, "Chapter 12");
    settings.Add("s10147", true, "Clock Tower", "_12");
    settings.Add("g537594863", true, "Scaffolds", "_12");
    settings.Add("s10051", true, "Salazar", "_12");
    settings.Add("s20115", true, "Salazar (Dead)", "_12");
    settings.Add("c25100", true, "End", "_12");

    settings.Add("_13", true, "Chapter 13");
    settings.Add("g539033905", true, "Gate 1", "_13");
    settings.Add("g539165221", true, "Gate 2", "_13");
    settings.Add("s10056", true, "Ashley", "_13");
    settings.Add("k119211200", true, "Level 1 Keycard", "_13");
    settings.Add("k119212800", true, "Level 2 Keycard", "_13");
    settings.Add("k119214400", true, "Level 3 Keycard", "_13");
    settings.Add("c25200", true, "End", "_13");

    settings.Add("_14", true, "Chapter 14");
    settings.Add("s10061", true, "Amber", "_14");
    settings.Add("s10062", true, "Krauser", "_14");
    settings.Add("s10063", true, "Krauser Transform", "_14");
    settings.Add("s10136", true, "Krauser 2", "_14");
    settings.Add("c25300", true, "End", "_14");

    settings.Add("_15", true, "Chapter 15");
    settings.Add("s20130", true, "Mike Finds a Way", "_15");
    settings.Add("s20128", true, "Mike Opens the Gates", "_15");
    settings.Add("s10066", true, "Goodbye Mike", "_15");
    settings.Add("s10067", true, "Altar", "_15");
    settings.Add("c25400", true, "End", "_15");

    settings.Add("_16", true, "Chapter 16");
    settings.Add("s10071", true, "Saddler", "_16");
    settings.Add("s20119", true, "Saddler 2", "_16");
    settings.Add("s10158", true, "Saddler (Dead)", "_16");
    settings.Add("s10074", true, "Jet Ski", "_16");
    settings.Add("s10076", true, "End", "_16");
}

split
{
    current.KeyItems = new HashSet<int>();
    for (int i = 0; i < current.KeyItemsCount; i++)
    {
        int KeyItemId = new DeepPointer(0x0D2320C8, 0x40, 0x30, 0x318, 0x98, 0x60, 0x20 + 0x8 * i, 0x10, 0x28).Deref<int>(game);
        current.KeyItems.Add(KeyItemId);
    }

    foreach (int z in current.KeyItems)
    {
        if (!old.KeyItems.Contains(z))
        {
            string x = String.Format("k{0}", (uint)z);
            if (settings.ContainsKey(x) && !vars.Done.Contains(x))
            {
                vars.Done.Add(x);
                return true;
            }
        }
    }

    if (old.ChapterID != current.ChapterID)
    {
        string x = String.Format("c{0}", (uint)current.ChapterID);
        if (settings.ContainsKey(x) && !vars.Done.Contains(x))
        {
            vars.Done.Add(x);
            return true;
        }
    }

    if (old.CutSceneId != current.CutSceneId)
    {
        string x = String.Format("s{0}", (uint)current.CutSceneId);
        if (settings.ContainsKey(x) && !vars.Done.Contains(x))
        {
            vars.Done.Add(x);
            return true;
        }
    }

    if (old.InteractionId != current.InteractionId)
    {
        string x = String.Format("g{0}", (uint)current.InteractionId);
        if (settings.ContainsKey(x) && !vars.Done.Contains(x))
        {
            vars.Done.Add(x);
            return true;
        }
    }
}

update
{
    if (old.PrevChapterStatsCount < current.PrevChapterStatsCount)
    {
        vars.ChapterEnd = true;
    }

    if (vars.ChapterEnd && old.ChapterStartTime != current.ChapterStartTime && current.ChapterStartTime != 0)
    {
        vars.ChapterEnd = false;
    }
}

gameTime
{
    long time = 0;
    for (int i = 0; i < current.PrevChapterStatsCount; i++)
    {
        time += new DeepPointer(0x0D20FF80, 0x38, 0x10, 0x20 + 0x8 * i, 0x18, 0x18).Deref<long>(game);
    }

    if (current.ChapterID != -1 && current.PrevChapterStatsCount < 16 && !vars.ChapterEnd)
        time += current.GameElapsedTime - current.DemoSpendingTime - current.PauseSpendingTime - current.ChapterStartTime;

    return TimeSpan.FromTicks(10 * time);
}

reset
{
    return old.CutSceneId == -1 && current.CutSceneId == 10157;
}

start
{
    if (old.CutSceneId != current.CutSceneId && current.CutSceneId == 10003)
    {
        vars.Done.Clear();
        vars.ChapterEnd = false;
        return true;
    }
}

isLoading
{
    return true;
}
