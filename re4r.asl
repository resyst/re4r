// re4r autosplitter

state("re4") {}

// CutsceneId : sXXXX
// StageId : zXXXX
// KeyItemId : kXXXX
// ChapterId : cXXXX
// GimmickId : gXXXX
// Manual Combo: mXXXX
startup
{
    settings.Add("_1", true, "Chapter 1");
    settings.CurrentDefaultParent = "_1";
    settings.Add("s27AB", true, "Defenestration");
    settings.Add("z9D08", true, "Village");
    settings.Add("s271B", true, "Bingo");
    settings.Add("k71B6280", true, "Wooden Cog");
    settings.Add("zA988", true, "Tunnel Exit");
    settings.Add("c52D0", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_2", true, "Chapter 2");
    settings.CurrentDefaultParent = "_2";
    settings.Add("s2790", true, "Merchant");
    settings.Add("k71AE580", true, "Hexagonal Emblem");
    settings.Add("zACA9", true, "Eagle Gate");
    settings.Add("zAC4E", true, "Village Chief's Manor");
    settings.Add("c5334", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_3", true, "Chapter 3");
    settings.CurrentDefaultParent = "_3";
    settings.Add("s2799", true, "Church");
    settings.Add("zB1BC", true, "Lake");
    settings.Add("k71BF880", true, "Boat Fuel");
    settings.Add("s2722", true, "Del Lago");
    settings.Add("c5654", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_4", true, "Chapter 4");
    settings.CurrentDefaultParent = "_4";
    settings.Add("k71B4FC0", true, "Blasphemer's Head");
    settings.Add("k71B5600", true, "Apostate's Head");
    settings.Add("k71BBA00", true, "Church Insignia");
    settings.Add("s2726", true, "El Gigante");
    settings.Add("s4E9B", true, "El Gigante (Dead)");
    settings.Add("c56B8", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_5", true, "Chapter 5");
    settings.CurrentDefaultParent = "_5";
    settings.Add("m0", true, "Village");
    settings.Add("s272A", true, "Villa");
    settings.Add("c571C", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_6", true, "Chapter 6");
    settings.CurrentDefaultParent = "_6";
    settings.Add("g201102CD", true, "Quarry");
    settings.Add("s278D", true, "Chainsaw Sisters");
    settings.Add("g202202A5", true, "Crank Door");
    settings.Add("s278E", true, "Mendez Chase");
    settings.Add("zB929", true, "Merchant Skip");
    settings.Add("c5A3C", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_7", true, "Chapter 7");
    settings.CurrentDefaultParent = "_7";
    settings.Add("s2778", true, "Catapults");
    settings.Add("s2730", true, "Salazar Greeting");
    settings.Add("zC5A9", true, "Water Hall");
    settings.Add("k71AF200", true, "Halo Wheel");
    settings.Add("c5AA0", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_8", true, "Chapter 8");
    settings.CurrentDefaultParent = "_8";
    settings.Add("k71AFE80", true, "Crimson Lantern");
    settings.Add("s2734", true, "Ada");
    settings.Add("s4E91", true, "Gigante Ramparts");
    settings.Add("c5B04", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_9", true, "Chapter 9");
    settings.CurrentDefaultParent = "_9";
    settings.Add("g200B0B4E", true, "Flag 1");
    settings.Add("g200B0B50", true, "Flag 2");
    settings.Add("g200B0B4F", true, "Flag 3");
    settings.Add("zCB84", true, "Grand Hall");
    settings.Add("k71B3D00", true, "Serpent Head");
    settings.Add("k71B3080", true, "Lion Head");
    settings.Add("zCBE9", true, "Armory");
    settings.Add("k71B36C0", true, "Goat Head");
    settings.Add("s2736", true, "Ashley");
    settings.Add("k71B2A40", true, "Salazar Family Insignia");
    settings.Add("c5E24", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_10", true, "Chapter 10");
    settings.CurrentDefaultParent = "_10";
    settings.Add("g200B0E53", true, "Ladder Switch");
    settings.Add("g200B0E52", true, "Stairway Switch");
    settings.Add("zD41C", true, "Twin Garradors");
    settings.Add("s2739", true, "Throne Room");
    settings.Add("zD481", true, "Leaving the Depths");
    settings.Add("s273A", true, "Verdugo");
    settings.Add("g200B0BE3", false, "Elevator Button");
    settings.Add("c5E88", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_11", true, "Chapter 11");
    settings.CurrentDefaultParent = "_11";
    settings.Add("k71B68C0", true, "Dynamite");
    settings.Add("s273E", true, "Double Gigante");
    settings.Add("g2022027E", true, "Furnace Exit");
    settings.Add("g201F0100", true, "Cart 1");
    settings.Add("zD7A2", true, "Stopover");
    settings.Add("g201F0129", true, "Cart 2");
    settings.Add("s273F", true, "Jump");
    settings.Add("s2740", true, "Goodbye Luis");
    settings.Add("c5EEC", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_12", true, "Chapter 12");
    settings.CurrentDefaultParent = "_12";
    settings.Add("s27A3", true, "Clock Tower");
    settings.Add("g200B0DD2", true, "Start Lift");
    settings.Add("zDB28", true, "Scaffolds");
    settings.Add("s2743", true, "Salazar");
    settings.Add("s4E93", true, "Salazar (Dead)");
    settings.Add("c620C", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_13", true, "Chapter 13");
    settings.CurrentDefaultParent = "_13";
    settings.Add("zEAC7", true, "Gate 1");
    settings.Add("zEB28", true, "Gate 2");
    settings.Add("s2748", true, "Rescue Ashley");
    settings.Add("k71B04C0", true, "Level 1 Keycard");
    settings.Add("k71B0B00", true, "Level 2 Keycard");
    settings.Add("k71B1140", true, "Level 3 Keycard");
    settings.Add("c6270", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_14", true, "Chapter 14");
    settings.CurrentDefaultParent = "_14";
    settings.Add("s274D", true, "Amber");
    settings.Add("s274E", true, "Krauser");
    settings.Add("s274F", true, "Krauser Transform");
    settings.Add("s2798", true, "Krauser 2");
    settings.Add("c62D4", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_15", true, "Chapter 15");
    settings.CurrentDefaultParent = "_15";
    settings.Add("s4EA2", true, "Mike Finds a Way");
    settings.Add("s4EA0", false, "Mike Opens the Gates");
    settings.Add("z10237", true, "Bulwark");
    settings.Add("g202000D9", true, "Right Switch");
    settings.Add("g202000D8", true, "Left Switch");
    settings.Add("s2752", true, "Goodbye Mike");
    settings.Add("z10620", true, "Sanctuary");
    settings.Add("s2753", true, "Altar");
    settings.Add("c6338", true, "End");
    settings.CurrentDefaultParent = null;

    settings.Add("_16", true, "Chapter 16");
    settings.CurrentDefaultParent = "_16";
    settings.Add("s2757", true, "Saddler");
    settings.Add("s4E97", true, "Saddler 2");
    settings.Add("s27AE", true, "Saddler (Dead)");
    settings.Add("s275A", true, "Jet Ski");
    settings.Add("s275C", true, "End");
}

init
{
    var s = new SignatureScanner(game, modules.First().BaseAddress, modules.First().ModuleMemorySize);

    var p = s.Scan(new SigScanTarget(3, "48 8b 15 ?? ?? ?? ?? 48 8b c8 49 8b d8"));
    p += game.ReadValue<int>(p) + 4;
    vars.GameElapsedTime = new MemoryWatcher<long>(new DeepPointer(p, 0x20, 0x18));
    vars.DemoSpendingTime = new MemoryWatcher<long>(new DeepPointer(p, 0x20, 0x20));
    vars.PauseSpendingTime = new MemoryWatcher<long>(new DeepPointer(p, 0x20, 0x30));

    p = s.Scan(new SigScanTarget(6, "48 8b d9 48 8b 15 ?? ?? ?? ?? e8 ?? ?? ?? ?? 85 c0"));
    p += game.ReadValue<int>(p) + 4;
    vars.ChapterStartTime = new MemoryWatcher<long>(new DeepPointer(p, 0x20, 0x10, 0x18));
    vars.ChapterHistoryList = new MemoryWatcher<IntPtr>(new DeepPointer(p, 0x38));

    p = s.Scan(new SigScanTarget(3, "48 8b 15 ?? ?? ?? ?? 44 8b c6 49 8b cd"));
    p += game.ReadValue<int>(p) + 4;
    vars.ChapterId = new MemoryWatcher<int>(new DeepPointer(p, 0x154));
    vars.CutsceneId = new MemoryWatcher<int>(new DeepPointer(p, 0x17C));

    p = s.Scan(new SigScanTarget(6, "48 8b f8 48 8b 15 ?? ?? ?? ?? 48 8b cb e8"));
    p += game.ReadValue<int>(p) + 4;
    vars.PlayerList = new MemoryWatcher<IntPtr>(new DeepPointer(p, 0x90));

    vars.WatcherList = new MemoryWatcherList()
    {
        vars.GameElapsedTime,
        vars.DemoSpendingTime,
        vars.PauseSpendingTime,
        vars.ChapterStartTime,
        vars.ChapterHistoryList,
        vars.ChapterId,
        vars.CutsceneId,
        vars.PlayerList
    };

    vars.PrevTotalTime = 0;
    vars.IsChapterEnd = false;
    current.ChapterHistoryListSize = 0;
    current.Interactions = new List<int>();
    current.KeyItems = new List<int>();
    current.StageId = -1;
    vars.Done = new HashSet<string>();
}

update
{
    vars.WatcherList.UpdateAll(game);

    current.ChapterHistoryListSize = game.ReadValue<int>((IntPtr)vars.ChapterHistoryList.Current + 0x18);
    if (old.ChapterHistoryListSize != current.ChapterHistoryListSize)
    {
        vars.IsChapterEnd = true;
        vars.PrevTotalTime = 0L;
        for (int i = 0; i < current.ChapterHistoryListSize; i++)
        {
            vars.PrevTotalTime += new DeepPointer(vars.ChapterHistoryList.Current + 0x10, 0x20 + 0x8 * i, 0x18, 0x18).Deref<long>(game);
        }
    }
    vars.IsChapterEnd &= !(vars.ChapterStartTime.Old != vars.ChapterStartTime.Current && vars.ChapterStartTime.Current != 0);

    current.Interactions = new List<int>();
    current.KeyItems = new List<int>();
    current.StageId = -1;
    var PlayerListSize = game.ReadValue<int>((IntPtr)vars.PlayerList.Current + 0x18);
    if (PlayerListSize != 0)
    {
        for (int i = 0; i < 4; i++)
        {
            var InteractionId = new DeepPointer(vars.PlayerList.Current + 0x10, 0x20, 0x10, 0x10, 0x20 + 0x4 * i).Deref<int>(game);
            if (InteractionId != -1)
            {
                current.Interactions.Add(InteractionId);
            }
        }

        var KeyItemList = new DeepPointer(vars.PlayerList.Current + 0x10, 0x20, 0x250, 0x318, 0x98, 0x20).Deref<IntPtr>(game);
        var KeyItemListSize = game.ReadValue<int>(KeyItemList + 0x18);
        for (int i = 0; i < KeyItemListSize; i++)
        {
            var KeyItemId = new DeepPointer(KeyItemList + 0x10, 0x20 + 0x8 * i, 0x10, 0x28).Deref<int>(game);
            current.KeyItems.Add(KeyItemId);
        }

        current.StageId = new DeepPointer(vars.PlayerList.Current + 0x10, 0x20, 0x1A8).Deref<int>(game);
    }
}

isLoading
{
    return true;
}

gameTime
{
    var time = 0L;
    if (vars.ChapterStartTime.Current != -2)
    {
        time = vars.PrevTotalTime;
        if (!vars.IsChapterEnd)
        {
            time += vars.GameElapsedTime.Current - vars.DemoSpendingTime.Current - vars.PauseSpendingTime.Current - vars.ChapterStartTime.Current;
        }
    }
    return TimeSpan.FromTicks(10 * time);
}

reset
{
    return vars.CutsceneId.Old == -1 && vars.CutsceneId.Current == 0x27AD;
}

split
{
    var Delta = new List<string>();

    if (vars.CutsceneId.Old != vars.CutsceneId.Current)
    {
        Delta.Add(string.Format("s{0:X}", vars.CutsceneId.Current));
    }

    if (old.StageId != current.StageId)
    {
        Delta.Add(string.Format("z{0:X}", current.StageId));
        
        if (vars.ChapterId.Current == 0x56B8 && current.StageId == 0x9D08)
        {
            Delta.Add("m0");
        }
    }

    foreach (int x in current.KeyItems)
    {
        if (!old.KeyItems.Contains(x))
        {
            Delta.Add(string.Format("k{0:X}", x));
        }
    }

    if (vars.ChapterId.Old != vars.ChapterId.Current)
    {
        Delta.Add(string.Format("c{0:X}", vars.ChapterId.Current));
    }

    foreach (int x in current.Interactions)
    {
        if (!old.Interactions.Contains(x))
        {
            Delta.Add(string.Format("g{0:X}", x));
        }
    }

    foreach (string s in Delta)
    {
        if (settings.ContainsKey(s) && settings[s] && !vars.Done.Contains(s))
        {
            vars.Done.Add(s);
            return true;
        }
    }
}

start
{
    if (vars.CutsceneId.Old != vars.CutsceneId.Current && vars.CutsceneId.Current == 0x2713)
    {
        vars.PrevTotalTime = 0;
        vars.IsChapterEnd = false;
        vars.Done = new HashSet<string>();
        return true;
    }
}
